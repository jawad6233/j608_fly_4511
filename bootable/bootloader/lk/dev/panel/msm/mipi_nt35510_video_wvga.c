/* Copyright (c) 2012, Code Aurora Forum. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above
 *       copyright notice, this list of conditions and the following
 *       disclaimer in the documentation and/or other materials provided
 *       with the distribution.
 *     * Neither the name of Code Aurora Forum, Inc. nor the names of its
 *       contributors may be used to endorse or promote products derived
 *       from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
 * IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include <stdint.h>
#include <msm_panel.h>
#include <mipi_dsi.h>
#include <sys/types.h>
#include <err.h>
#include <reg.h>
#include <debug.h>
#include <platform/iomap.h>
#include <target/display.h>
#include <dev/gpio.h>


extern int mipi_nt35510_panel_wvga_rotate();
/* MIPI NT35510 panel commands */
static char exit_sleep[4] = {0x11, 0x00, 0x09, 0x80};
static char display_on[4] = {0x29, 0x00, 0x05, 0x80};

static char video0[12] = {
	0x06, 0x00, 0x29, 0xc0, 0xF0, 0x55, 0xAA, 0x52,
	0x08, 0x01, 0xff, 0xff,
};
static char video1[8] = {
	0x04, 0x00, 0x29, 0xc0, 0xBC, 0x00, 0xA0, 0x00,
};
static char video2[8] = {
	0x04, 0x00, 0x29, 0xc0, 0xBD, 0x00, 0xA0, 0x00,
};
static char video3[8] = {
	0x03, 0x00, 0x29, 0xc0, 0xBE, 0x00, 0x79, 0xff,
};
static char video4[60] = {
	0x35, 0x00, 0x29, 0xc0, 0xD1, 0x00, 0x00, 0x00,
	0x14, 0x00, 0x32, 0x00,
	0x4F, 0x00, 0x65, 0x00,
	0x8B, 0x00, 0xA8, 0x00,
	0xD5, 0x00, 0xF7, 0x01,
	0x2B, 0x01, 0x54, 0x01,
	0x8E, 0x01, 0xBB, 0x01,
	0xBC, 0x01, 0xE3, 0x02,
	0x08, 0x02, 0x1C, 0x02,
	0x39, 0x02, 0x4F, 0x02,
	0x76, 0x02, 0xA3, 0x02,
	0xE3, 0x03, 0x12, 0x03,
	0x4C, 0x03, 0x66, 0x03,
	0x9A, 0xff, 0xff, 0xff,
};
static char video5[60] = {
	0x35, 0x00, 0x29, 0xc0, 0xD2, 0x00, 0x00, 0x00,
	0x14, 0x00, 0x32, 0x00,
	0x4F, 0x00, 0x65, 0x00,
	0x8B, 0x00, 0xA8, 0x00,
	0xD5, 0x00, 0xF7, 0x01,
	0x2B, 0x01, 0x54, 0x01,
	0x8E, 0x01, 0xBB, 0x01,
	0xBC, 0x01, 0xE3, 0x02,
	0x08, 0x02, 0x1C, 0x02,
	0x39, 0x02, 0x4F, 0x02,
	0x76, 0x02, 0xA3, 0x02,
	0xE3, 0x03, 0x12, 0x03,
	0x4C, 0x03, 0x66, 0x03,
	0x9A, 0xff, 0xff, 0xff
};
static char video6[60] = {
	0x35, 0x00, 0x29, 0xc0, 0xD3, 0x00, 0x00, 0x00,
	0x14, 0x00, 0x32, 0x00,
	0x4F, 0x00, 0x65, 0x00,
	0x8B, 0x00, 0xA8, 0x00,
	0xD5, 0x00, 0xF7, 0x01,
	0x2B, 0x01, 0x54, 0x01,
	0x8E, 0x01, 0xBB, 0x01,
	0xBC, 0x01, 0xE3, 0x02,
	0x08, 0x02, 0x1C, 0x02,
	0x39, 0x02, 0x4F, 0x02,
	0x76, 0x02, 0xA3, 0x02,
	0xE3, 0x03, 0x12, 0x03,
	0x4C, 0x03, 0x66, 0x03,
	0x9A, 0xff, 0xff, 0xff,
};
static char video7[60] = {
	0x35, 0x00, 0x29, 0xc0, 0xD4, 0x00, 0x00, 0x00,
	0x14, 0x00, 0x32, 0x00,
	0x4F, 0x00, 0x65, 0x00,
	0x8B, 0x00, 0xA8, 0x00,
	0xD5, 0x00, 0xF7, 0x01,
	0x2B, 0x01, 0x54, 0x01,
	0x8E, 0x01, 0xBB, 0x01,
	0xBC, 0x01, 0xE3, 0x02,
	0x08, 0x02, 0x1C, 0x02,
	0x39, 0x02, 0x4F, 0x02,
	0x76, 0x02, 0xA3, 0x02,
	0xE3, 0x03, 0x12, 0x03,
	0x4C, 0x03, 0x66, 0x03,
	0x9A, 0xff, 0xff, 0xff,
};
static char video8[60] = {
	0x35, 0x00, 0x29, 0xc0, 0xD5, 0x00, 0x00, 0x00,
	0x14, 0x00, 0x32, 0x00,
	0x4F, 0x00, 0x65, 0x00,
	0x8B, 0x00, 0xA8, 0x00,
	0xD5, 0x00, 0xF7, 0x01,
	0x2B, 0x01, 0x54, 0x01,
	0x8E, 0x01, 0xBB, 0x01,
	0xBC, 0x01, 0xE3, 0x02,
	0x08, 0x02, 0x1C, 0x02,
	0x39, 0x02, 0x4F, 0x02,
	0x76, 0x02, 0xA3, 0x02,
	0xE3, 0x03, 0x12, 0x03,
	0x4C, 0x03, 0x66, 0x03,
	0x9A, 0xff, 0xff, 0xff,
};
static char video9[60] = {
	0x35, 0x00, 0x29, 0xc0, 0xD6, 0x00, 0x00, 0x00,
	0x14, 0x00, 0x32, 0x00,
	0x4F, 0x00, 0x65, 0x00,
	0x8B, 0x00, 0xA8, 0x00,
	0xD5, 0x00, 0xF7, 0x01,
	0x2B, 0x01, 0x54, 0x01,
	0x8E, 0x01, 0xBB, 0x01,
	0xBC, 0x01, 0xE3, 0x02,
	0x08, 0x02, 0x1C, 0x02,
	0x39, 0x02, 0x4F, 0x02,
	0x76, 0x02, 0xA3, 0x02,
	0xE3, 0x03, 0x12, 0x03,
	0x4C, 0x03, 0x66, 0x03,
	0x9A, 0xff, 0xff, 0xff,
};
static char video10[8] = {
	0x04, 0x00, 0x29, 0xc0, 0xB0, 0x0A, 0x0A, 0x0A,
};
static char video11[8] = {
	0x04, 0x00, 0x29, 0xc0, 0xB1, 0x0A, 0x0A, 0x0A,
};
static char video12[8] = {
	0x04, 0x00, 0x29, 0xc0, 0xBA, 0x24, 0x24, 0x24,
};
static char video13[8] = {
	0x04, 0x00, 0x29, 0xc0, 0xB9, 0x24, 0x24, 0x24,
};
static char video14[8] = {
	0x04, 0x00, 0x29, 0xc0, 0xB8, 0x24, 0x24, 0x24,
};
static char video15[12] = {
	0x06, 0x00, 0x29, 0xc0, 0xF0, 0x55, 0xAA, 0x52,
	0x08, 0x00, 0xff, 0xff,
};
static char video16[8] = {
	0x02, 0x00, 0x29, 0xc0, 0xB3, 0x00, 0xff, 0xff,
};
static char video17[8] = {
	0x02, 0x00, 0x29, 0xc0, 0xB4, 0x10, 0xff, 0xff,
};
static char video18[8] = {
	0x02, 0x00, 0x29, 0xc0, 0xB6, 0x02, 0xff, 0xff,
};
static char video19[8] = {
	0x03, 0x00, 0x29, 0xc0, 0xB1, 0xFC, 0x00, 0xff,
};
static char video20[8] = {
	0x04, 0x00, 0x29, 0xc0, 0xBC, 0x05, 0x05, 0x05,
};
static char video21[8] = {
	0x03, 0x00, 0x29, 0xc0, 0xB7, 0x20, 0x20, 0xff,
};

static char video22[12] = {
	0x05, 0x00, 0x29, 0xc0, 0xB8, 0x01, 0x03, 0x03,
	0x03, 0xff, 0xff, 0xff,
};
static char video23[24] = {
	0x13, 0x00, 0x29, 0xc0, 0xC8, 0x01, 0x00, 0x78,
	0x50, 0x78, 0x50, 0x78,
	0x50, 0x78, 0x50, 0xC8,
	0x3C, 0x3C, 0xC8, 0xC8,
	0x3C, 0x3C, 0xC8, 0xff,
};
static char video24[12] = {
	0x06, 0x00, 0x29, 0xc0, 0xBD, 0x01, 0x84, 0x07,
	0x31, 0x00, 0xff, 0xff,
};
static char video25[12] = {
	0x06, 0x00, 0x29, 0xc0, 0xBE, 0x01, 0x84, 0x07,
	0x31, 0x00, 0xff, 0xff,
};
static char video26[12] = {
	0x06, 0x00, 0x29, 0xc0, 0xBF, 0x01, 0x84, 0x07,
	0x31, 0x00, 0xff, 0xff
};
static char video27[8] = {
	0x02, 0x00, 0x29, 0xc0, 0x35, 0x00, 0xff, 0xff,
};

static struct mipi_dsi_cmd nt35510_panel_video_mode_cmds[] = {
	{sizeof(video0), video0},
	{sizeof(video1), video1},
	{sizeof(video2), video2},
	{sizeof(video3), video3},
	{sizeof(video4), video4},
	{sizeof(video5), video5},
	{sizeof(video6), video6},
	{sizeof(video7), video7},
	{sizeof(video8), video8},
	{sizeof(video9), video9},
	{sizeof(video10), video10},
	{sizeof(video11), video11},
	{sizeof(video12), video12},
	{sizeof(video13), video13},
	{sizeof(video14), video14},
	{sizeof(video15), video15},
	{sizeof(video16), video16},
	{sizeof(video17), video17},
	{sizeof(video18), video18},
	{sizeof(video19), video19},
	{sizeof(video20), video20},
	{sizeof(video21), video21},
	{sizeof(video22), video22},
	{sizeof(video23), video23},
	{sizeof(video24), video24},
	{sizeof(video25), video25},
	{sizeof(video26), video26},
	{sizeof(video27), video27},
	{sizeof(exit_sleep), exit_sleep},
	{sizeof(display_on), display_on},
};

int mipi_nt35510_video_wvga_config(void *pdata)
{
	int ret = NO_ERROR;
	/* 3 Lanes -- Enables Data Lane0, 1, 2 */
	unsigned char lane_en = 3;
	unsigned long low_pwr_stop_mode = 1;

	/* Needed or else will have blank line at top of display */
	unsigned char eof_bllp_pwr = 0x9;

	unsigned char interleav = 0;
	struct lcdc_panel_info *lcdc = NULL;
	struct msm_panel_info *pinfo = (struct msm_panel_info *) pdata;

	if (pinfo == NULL)
		return ERR_INVALID_ARGS;

	lcdc =  &(pinfo->lcdc);
	if (lcdc == NULL)
		return ERR_INVALID_ARGS;

	ret = mipi_dsi_video_mode_config((pinfo->xres),
			(pinfo->yres),
			(pinfo->xres),
			(pinfo->yres),
			(lcdc->h_front_porch),
			(lcdc->h_back_porch),
			(lcdc->v_front_porch),
			(lcdc->v_back_porch),
			(lcdc->h_pulse_width),
			(lcdc->v_pulse_width),
			pinfo->mipi.dst_format,
			pinfo->mipi.traffic_mode,
			lane_en,
			low_pwr_stop_mode,
			eof_bllp_pwr,
			interleav);
	return ret;
}

int mipi_nt35510_video_wvga_on()
{
	int ret = NO_ERROR;
	return ret;
}

int mipi_nt35510_video_wvga_off()
{
	int ret = NO_ERROR;
	return ret;
}

static struct mipi_dsi_phy_ctrl dsi_video_mode_phy_db = {
	/* DSI_BIT_CLK at 500MHz, 2 lane, RGB888 */
	{0x03, 0x01, 0x01, 0x00},	/* regulator */
	/* timing   */
	{0xb9, 0x8e, 0x1f, 0x00, 0x98, 0x9c, 0x22,
	0x90, 0x18, 0x03, 0x04},
	{0x7f, 0x00, 0x00, 0x00},	/* phy ctrl */
	{0xbb, 0x02, 0x06, 0x00},	/* strength */
	/* pll control */
	{0x00, 0xec, 0x31, 0xd2, 0x00, 0x40, 0x37, 0x62,
	0x01, 0x0f, 0x07,	/*  --> Two lane configuration */
	0x05, 0x14, 0x03, 0x0, 0x0, 0x0, 0x20, 0x0, 0x02, 0x0},
};

void mipi_nt35510_video_wvga_init(struct msm_panel_info *pinfo)
{
	if (!pinfo)
		return;

	pinfo->xres = NT35510_MIPI_FB_WIDTH;
	pinfo->yres = NT35510_MIPI_FB_HEIGHT;
	pinfo->lcdc.h_back_porch = MIPI_HSYNC_BACK_PORCH_DCLK;
	pinfo->lcdc.h_front_porch = MIPI_HSYNC_FRONT_PORCH_DCLK;
	pinfo->lcdc.h_pulse_width = MIPI_HSYNC_PULSE_WIDTH;
	pinfo->lcdc.v_back_porch = MIPI_VSYNC_BACK_PORCH_LINES;
	pinfo->lcdc.v_front_porch = MIPI_VSYNC_FRONT_PORCH_LINES;
	pinfo->lcdc.v_pulse_width = MIPI_VSYNC_PULSE_WIDTH;
	pinfo->mipi.num_of_lanes = 2;

	pinfo->type = MIPI_VIDEO_PANEL;
	pinfo->wait_cycle = 0;
	pinfo->bpp = 24;

	pinfo->mipi.mode = DSI_VIDEO_MODE;
	pinfo->mipi.traffic_mode = 2;
	pinfo->mipi.dst_format = DSI_VIDEO_DST_FORMAT_RGB888;
	pinfo->mipi.dsi_phy_db = &dsi_video_mode_phy_db;
	pinfo->mipi.tx_eot_append = TRUE;

	pinfo->mipi.lane_swap = 1;
	pinfo->mipi.panel_cmds = nt35510_panel_video_mode_cmds;
	pinfo->mipi.num_of_panel_cmds = \
			ARRAY_SIZE(nt35510_panel_video_mode_cmds);

	pinfo->on = mipi_nt35510_video_wvga_on;
	pinfo->off = mipi_nt35510_video_wvga_off;
	pinfo->config = mipi_nt35510_video_wvga_config;
	pinfo->rotate = mipi_nt35510_panel_wvga_rotate;

	return;
}
