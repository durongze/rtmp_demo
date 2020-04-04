/*
 *  Copyright (C) 2009 Andrej Stepanchuk
 *  Copyright (C) 2009-2010 Howard Chu
 *
 *  This file is part of librtmp.
 *
 *  librtmp is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as
 *  published by the Free Software Foundation; either version 2.1,
 *  or (at your option) any later version.
 *
 *  librtmp is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with librtmp see the file COPYING.  If not, write to
 *  the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 *  Boston, MA  02110-1301, USA.
 *  http://www.gnu.org/copyleft/lgpl.html
 */

#include <stdlib.h>
#include <string.h>

#include <assert.h>
#include <ctype.h>

#include "rtmp_sys.h"
#include "log.h"

#define RTMPUrlLog(l, fmt, ...) RTMPLog(l, "URL", fmt, ##__VA_ARGS__)

void RTMPChunk_Show(RTMPChunk chunk)
{
	RTMPUrlLog(RTMP_LOGDEBUG, "c_headerSize:%d\n", chunk.c_headerSize);
	RTMPUrlLog(RTMP_LOGDEBUG, "c_chunkSize:%d\n", chunk.c_chunkSize);
}

void RTMP_Show(RTMP rtmp)
{
	RTMPUrlLog(RTMP_LOGDEBUG, "m_inChunkSize:%d\n", rtmp.m_inChunkSize);
	RTMPUrlLog(RTMP_LOGDEBUG, "m_outChunkSize:%d\n", rtmp.m_outChunkSize);
	RTMPUrlLog(RTMP_LOGDEBUG, "m_nBWCheckCounter:%d\n", rtmp.m_nBWCheckCounter);
	RTMPUrlLog(RTMP_LOGDEBUG, "m_nBytesIn:%d\n", rtmp.m_nBytesIn);
	RTMPUrlLog(RTMP_LOGDEBUG, "m_nBytesInSent:%d\n", rtmp.m_nBytesInSent);
	RTMPUrlLog(RTMP_LOGDEBUG, "m_nBufferMS:%d\n", rtmp.m_nBufferMS);
	RTMPUrlLog(RTMP_LOGDEBUG, "m_stream_id:%d\n", rtmp.m_stream_id);		/* returned in _result from createStream */
	RTMPUrlLog(RTMP_LOGDEBUG, "m_mediaChannel:%d\n", rtmp.m_mediaChannel);
	RTMPUrlLog(RTMP_LOGDEBUG, "m_mediaStamp:%d\n", rtmp.m_mediaStamp);
	RTMPUrlLog(RTMP_LOGDEBUG, "m_pauseStamp:%d\n", rtmp.m_pauseStamp);
	RTMPUrlLog(RTMP_LOGDEBUG, "m_pausing:%d\n", rtmp.m_pausing);
	RTMPUrlLog(RTMP_LOGDEBUG, "m_nServerBW:%d\n", rtmp.m_nServerBW);
	RTMPUrlLog(RTMP_LOGDEBUG, "m_nClientBW:%d\n", rtmp.m_nClientBW);
	RTMPUrlLog(RTMP_LOGDEBUG, "m_nClientBW2:%d\n", rtmp.m_nClientBW2);
	RTMPUrlLog(RTMP_LOGDEBUG, "m_bPlaying:%d\n", rtmp.m_bPlaying);
	RTMPUrlLog(RTMP_LOGDEBUG, "m_bSendEncoding:%d\n", rtmp.m_bSendEncoding);
	RTMPUrlLog(RTMP_LOGDEBUG, "m_bSendCounter:%d\n", rtmp.m_bSendCounter);

	RTMPUrlLog(RTMP_LOGDEBUG, "m_numInvokes:%d\n", rtmp.m_numInvokes);
	RTMPUrlLog(RTMP_LOGDEBUG, "m_numCalls:%d\n", rtmp.m_numCalls);
	if (rtmp.m_methodCalls)
	RTMPUrlLog(RTMP_LOGDEBUG, "name:%s\n", rtmp.m_methodCalls->name.av_val);	/* remote method calls queue */

	if (rtmp.m_vecChannelsIn && *rtmp.m_vecChannelsIn)
		RTMPPacket_Dump(*rtmp.m_vecChannelsIn);
	if (rtmp.m_vecChannelsOut && *rtmp.m_vecChannelsOut)
		RTMPPacket_Dump(*rtmp.m_vecChannelsOut);
	if (rtmp.m_channelTimestamp)
	RTMPUrlLog(RTMP_LOGDEBUG, "m_channelTimestamp:%d\n", *rtmp.m_channelTimestamp);	/* abs timestamp of last packet */

	RTMPUrlLog(RTMP_LOGDEBUG, "m_fAudioCodecs:%f\n", rtmp.m_fAudioCodecs);	/* audioCodecs for the connect packet */
	RTMPUrlLog(RTMP_LOGDEBUG, "m_fVideoCodecs:%f\n", rtmp.m_fVideoCodecs);	/* videoCodecs for the connect packet */
	RTMPUrlLog(RTMP_LOGDEBUG, "m_fEncoding:%f\n", rtmp.m_fEncoding);		/* AMF0 or AMF3 */
	RTMPUrlLog(RTMP_LOGDEBUG, "m_fDuration:%f\n", rtmp.m_fDuration);		/* duration of stream in seconds */
	RTMPUrlLog(RTMP_LOGDEBUG, "m_msgCounter:%d\n", rtmp.m_msgCounter);		/* RTMPT stuff */
	RTMPUrlLog(RTMP_LOGDEBUG, "m_polling:%d\n", rtmp.m_polling);
	RTMPUrlLog(RTMP_LOGDEBUG, "m_resplen:%d\n", rtmp.m_resplen);
	RTMPUrlLog(RTMP_LOGDEBUG, "m_unackd:%d\n", rtmp.m_unackd);
    if (rtmp.m_clientID.av_val)
    RTMPUrlLog(RTMP_LOGDEBUG, "m_clientID:%s\n", rtmp.m_clientID.av_val);
    RTMPPacket_Dump(&rtmp.m_write);
    RTMPSockBuf_Dump(&rtmp.m_sb);
}

int RTMP_ParseURL(const char *url, int *protocol, AVal *host, unsigned int *port,
	AVal *playpath, AVal *app)
{
	char *p, *end, *col, *ques, *slash;

	RTMPUrlLog(RTMP_LOGDEBUG, "Parsing...");

	*protocol = RTMP_PROTOCOL_RTMP;
	*port = 0;
	playpath->av_len = 0;
	playpath->av_val = NULL;
	app->av_len = 0;
	app->av_val = NULL;

	/* Old School Parsing */

	/* look for usual :// pattern */
	p = strstr(url, "://");
	if(!p) {
		RTMPUrlLog(RTMP_LOGERROR, "RTMP URL: No :// in url!");
		return FALSE;
	}
	{
	int len = (int)(p-url);

	if(len == 4 && strncasecmp(url, "rtmp", 4)==0)
		*protocol = RTMP_PROTOCOL_RTMP;
	else if(len == 5 && strncasecmp(url, "rtmpt", 5)==0)
		*protocol = RTMP_PROTOCOL_RTMPT;
	else if(len == 5 && strncasecmp(url, "rtmps", 5)==0)
	        *protocol = RTMP_PROTOCOL_RTMPS;
	else if(len == 5 && strncasecmp(url, "rtmpe", 5)==0)
	        *protocol = RTMP_PROTOCOL_RTMPE;
	else if(len == 5 && strncasecmp(url, "rtmfp", 5)==0)
	        *protocol = RTMP_PROTOCOL_RTMFP;
	else if(len == 6 && strncasecmp(url, "rtmpte", 6)==0)
	        *protocol = RTMP_PROTOCOL_RTMPTE;
	else if(len == 6 && strncasecmp(url, "rtmpts", 6)==0)
	        *protocol = RTMP_PROTOCOL_RTMPTS;
	else {
		RTMPUrlLog(RTMP_LOGWARNING, "Unknown protocol!\n");
		goto parsehost;
	}
	}

	RTMPUrlLog(RTMP_LOGDEBUG, "Parsed protocol: %d", *protocol);

parsehost:
	/* let's get the hostname */
	p+=3;

	/* check for sudden death */
	if(*p==0) {
		RTMPUrlLog(RTMP_LOGWARNING, "No hostname in URL!");
		return FALSE;
	}

	end   = p + strlen(p);
	col   = strchr(p, ':');
	ques  = strchr(p, '?');
	slash = strchr(p, '/');

	{
	int hostlen;
	if(slash)
		hostlen = slash - p;
	else
		hostlen = end - p;
	if(col && col -p < hostlen)
		hostlen = col - p;

	if(hostlen < 256) {
		host->av_val = p;
		host->av_len = hostlen;
		RTMPUrlLog(RTMP_LOGDEBUG, "Parsed host    : %.*s", hostlen, host->av_val);
	} else {
		RTMPUrlLog(RTMP_LOGWARNING, "Hostname exceeds 255 characters!");
	}

	p+=hostlen;
	}

	/* get the port number if available */
	if(*p == ':') {
		unsigned int p2;
		p++;
		p2 = atoi(p);
		if(p2 > 65535) {
			RTMPUrlLog(RTMP_LOGWARNING, "Invalid port number!");
		} else {
			*port = p2;
		}
	}

	if(!slash) {
		RTMPUrlLog(RTMP_LOGWARNING, "No application or playpath in URL!");
		return TRUE;
	}
	p = slash+1;

	{
	/* parse application
	 *
	 * rtmp://host[:port]/app[/appinstance][/...]
	 * application = app[/appinstance]
	 */

	char *slash2, *slash3 = NULL, *slash4 = NULL;
	int applen, appnamelen;

	slash2 = strchr(p, '/');
	if(slash2)
		slash3 = strchr(slash2+1, '/');
	if(slash3)
		slash4 = strchr(slash3+1, '/');

	applen = end-p; /* ondemand, pass all parameters as app */
	appnamelen = applen; /* ondemand length */

	if(ques && strstr(p, "slist=")) { /* whatever it is, the '?' and slist= means we need to use everything as app and parse plapath from slist= */
		appnamelen = ques-p;
	}
	else if(strncmp(p, "ondemand/", 9)==0) {
                /* app = ondemand/foobar, only pass app=ondemand */
                applen = 8;
                appnamelen = 8;
        }
	else { /* app!=ondemand, so app is app[/appinstance] */
		if(slash4)
			appnamelen = slash4-p;
		else if(slash3)
			appnamelen = slash3-p;
		else if(slash2)
			appnamelen = slash2-p;

		applen = appnamelen;
	}

	app->av_val = p;
	app->av_len = applen;
	RTMPUrlLog(RTMP_LOGDEBUG, "Parsed app     : %.*s", applen, p);

	p += appnamelen;
	}

	if (*p == '/')
		p++;

	if (end-p) {
		AVal av = {p, end-p};
		RTMP_ParsePlaypath(&av, playpath);
	}

	return TRUE;
}

/*
 * Extracts playpath from RTMP URL. playpath is the file part of the
 * URL, i.e. the part that comes after rtmp://host:port/app/
 *
 * Returns the stream name in a format understood by FMS. The name is
 * the playpath part of the URL with formatting depending on the stream
 * type:
 *
 * mp4 streams: prepend "mp4:", remove extension
 * mp3 streams: prepend "mp3:", remove extension
 * flv streams: remove extension
 */
void RTMP_ParsePlaypath(AVal *in, AVal *out) {
	int addMP4 = 0;
	int addMP3 = 0;
	int subExt = 0;
	const char *playpath = in->av_val;
	const char *temp, *q, *ext = NULL;
	const char *ppstart = playpath;
	char *streamname, *destptr, *p;

	int pplen = in->av_len;

	out->av_val = NULL;
	out->av_len = 0;

	if ((*ppstart == '?') &&
	    (temp=strstr(ppstart, "slist=")) != 0) {
		ppstart = temp+6;
		pplen = strlen(ppstart);

		temp = strchr(ppstart, '&');
		if (temp) {
			pplen = temp-ppstart;
		}
	}

	q = strchr(ppstart, '?');
	if (pplen >= 4) {
		if (q)
			ext = q-4;
		else
			ext = &ppstart[pplen-4];
		if ((strncmp(ext, ".f4v", 4) == 0) ||
		    (strncmp(ext, ".mp4", 4) == 0)) {
			addMP4 = 1;
			subExt = 1;
		/* Only remove .flv from rtmp URL, not slist params */
		} else if ((ppstart == playpath) &&
		    (strncmp(ext, ".flv", 4) == 0)) {
			subExt = 1;
		} else if (strncmp(ext, ".mp3", 4) == 0) {
			addMP3 = 1;
			subExt = 1;
		}
	}

	streamname = (char *)malloc((pplen+4+1)*sizeof(char));
	if (!streamname)
		return;

	destptr = streamname;
	if (addMP4) {
		if (strncmp(ppstart, "mp4:", 4)) {
			strcpy(destptr, "mp4:");
			destptr += 4;
		} else {
			subExt = 0;
		}
	} else if (addMP3) {
		if (strncmp(ppstart, "mp3:", 4)) {
			strcpy(destptr, "mp3:");
			destptr += 4;
		} else {
			subExt = 0;
		}
	}

 	for (p=(char *)ppstart; pplen >0;) {
		/* skip extension */
		if (subExt && p == ext) {
			p += 4;
			pplen -= 4;
			continue;
		}
		if (*p == '%') {
			unsigned int c;
			sscanf(p+1, "%02x", &c);
			*destptr++ = c;
			pplen -= 3;
			p += 3;
		} else {
			*destptr++ = *p++;
			pplen--;
		}
	}
	*destptr = '\0';

	out->av_val = streamname;
	out->av_len = destptr - streamname;
}
