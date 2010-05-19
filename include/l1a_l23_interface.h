/* Messages to be sent between the different layers */

/* (C) 2010 by Harald Welte <laforge@gnumonks.org>
 * (C) 2010 by Holger Hans Peter Freyther
 *
 * All Rights Reserved
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 */

#ifndef l1a_l23_interface_h
#define l1a_l23_interface_h

#define L1CTL_FBSB_REQ		1
#define L1CTL_FBSB_RESP		2
#define L1CTL_DATA_IND		3
#define L1CTL_RACH_REQ		4
#define L1CTL_DM_EST_REQ	5
#define L1CTL_DATA_REQ		7
#define L1CTL_RESET		8
#define L1CTL_PM_REQ		9	/* power measurement */
#define L1CTL_PM_RESP		10	/* power measurement */
#define L1CTL_ECHO_REQ		11
#define L1CTL_ECHO_RESP		12

/*
 * NOTE: struct size. We do add manual padding out of the believe
 * that it will avoid some unaligned access.
 */

/* there are no more messages in a sequence */
#define L1CTL_F_DONE	0x01

struct l1ctl_hdr {
	uint8_t msg_type;
	uint8_t flags;
	uint8_t data[0];
} __attribute__((packed));

/*
 * downlink info ... down from the BTS..
 */
struct l1ctl_info_dl {
	/* GSM 08.58 channel number (9.3.1) */
	uint8_t chan_nr;
	/* GSM 08.58 link identifier (9.3.2) */
	uint8_t link_id;

	uint32_t frame_nr;

	/* the ARFCN and the band. FIXME: what about MAIO? */
	uint16_t band_arfcn;

	uint8_t rx_level;	/* 0 .. 63 in typical GSM notation (dBm+110) */
	uint8_t snr;		/* Signal/Noise Ration (dB) */
	uint8_t payload[0];
} __attribute__((packed));

/* new CCCH was found. This is following the header */
struct l1ctl_fbsb_resp {
	uint16_t band_arfcn;
	int16_t initial_freq_err;
	uint8_t result;
	uint8_t bsic;
	uint8_t padding[2];
	/* FIXME: contents of cell_info ? */
} __attribute__((packed));

/* data on the CCCH was found. This is following the header */
struct l1ctl_data_ind {
	uint8_t data[23];
} __attribute__((packed));

/*
 * uplink info
 */
struct l1ctl_info_ul {
	/* GSM 08.58 channel number (9.3.1) */
	uint8_t chan_nr;
	/* GSM 08.58 link identifier (9.3.2) */
	uint8_t link_id;

	uint8_t tx_power;
	uint8_t padding2;

	uint8_t payload[0];
} __attribute__((packed));

/*
 * msg for FBSB_REQ
 * the l1_info_ul header is in front
 */
struct l1ctl_fbsb_req {
	uint16_t band_arfcn;
	uint16_t timeout;	/* in TDMA frames */
	uint16_t freq_err_thresh1;
	uint16_t freq_err_thresh2;
	uint8_t num_freqerr_avg;
	uint8_t flags;		/* L1CTL_FBSB_F_* */
	uint8_t sync_info_idx;
	uint8_t reserved;
} __attribute__((packed));

#define L1CTL_FBSB_F_FB0	(1 << 0)
#define L1CTL_FBSB_F_FB1	(1 << 1)
#define L1CTL_FBSB_F_SB		(1 << 2)

/* the l1_info_ul header is in front */
struct l1ctl_rach_req {
	uint8_t ra;
	uint8_t padding[3];
} __attribute__((packed));

struct l1ctl_dm_est_req {
	uint16_t band_arfcn;
	union {
		struct {
			uint8_t maio_high:4,
				 h:1,
				 tsc:3;
			uint8_t hsn:6,
				 maio_low:2;
		} h1;
		struct {
			uint8_t arfcn_high:2,
				 spare:2,
				 h:1,
				 tsc:3;
			uint8_t arfcn_low;
		} h0;
	};
} __attribute__((packed));

struct l1ctl_pm_req {
	uint8_t type;
	uint8_t padding2;

	union {
		struct {
			uint16_t band_arfcn_from;
			uint16_t band_arfcn_to;
		} range;
	};
} __attribute__((packed));

/* a single L1CTL_PM response */
struct l1ctl_pm_resp {
	uint16_t band_arfcn;
	uint8_t pm[2];
} __attribute__((packed));

#endif
