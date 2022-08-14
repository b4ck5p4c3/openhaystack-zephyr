#include <zephyr/types.h>
#include <stddef.h>
#include <sys/printk.h>
#include <sys/util.h>

#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>

#define DEVICE_NAME CONFIG_BT_DEVICE_NAME
#define DEVICE_NAME_LEN (sizeof(DEVICE_NAME) - 1)

static char public_key[28] = {0x80, 0x0a, 0x3e, 0xa2, 0x4f, 0xdb, 0x85, 0xb6, 0xf9, 0x79, 0xc5, 0xd6, 0xca, 0x00, 0x2e, 0x20, 0xf7, 0x9a, 0xd6, 0x31, 0x51, 0x9c, 0xb1, 0xf2, 0x47, 0x3b, 0x6b, 0x5b};

/** Advertisement payload */
static unsigned char adv_data[31] = {
    0x1e, /* Length (30) */
    0xff, /* Manufacturer Specific Data (type 0xff) */
    0x4c, 0x00, /* Company ID (Apple) */
    0x12, 0x19, /* Offline Finding type and length */
    0x00, /* State */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, /* First two bits */
    0x00, /* Hint (0x00) */
};

void set_payload_from_key(uint8_t *payload, uint8_t *public_key) {
  /* copy last 22 bytes */
  memcpy(&payload[7], &public_key[6], 22);
  /* append two bits of public key */
  payload[29] = public_key[0] >> 6;
}

static void bt_ready(int err)
{
	if (err) {
		printk("Bluetooth init failed (err %d)\n", err);
		return;
	}

	char addr_s[BT_ADDR_LE_STR_LEN];
	bt_addr_le_t addr = {
		// public_key[0] | 0b11000000,
		// public_key[1],
		// public_key[2],
		// public_key[3],
		// public_key[4],
		// public_key[5]
		0
	};

	printk("Bluetooth initialized\n");

	set_payload_from_key(adv_data, public_key);

	struct bt_data ad[] = {
		BT_DATA_BYTES(BT_DATA_FLAGS, BT_LE_AD_NO_BREDR),
		BT_DATA_BYTES(BT_DATA_SVC_DATA16, &adv_data)
	};

	static const struct bt_data sd[] = {
		BT_DATA(BT_DATA_NAME_COMPLETE, DEVICE_NAME, DEVICE_NAME_LEN),
	};

	/* Start advertising */
	err = bt_le_adv_start(BT_LE_ADV_NCONN_IDENTITY, ad, ARRAY_SIZE(ad),
			      sd, ARRAY_SIZE(sd));
	if (err) {
		printk("Advertising failed to start (err %d)\n", err);
		return;
	}

	size_t idCount = 1;
	bt_id_get(&addr, &idCount);
	bt_addr_le_to_str(&addr, addr_s, sizeof(addr_s));

	printk("Beacon started, advertising as %s\n", addr_s);
}

void main(void)
{
	int err;

	printk("Starting Beacon Demo\n");

	/* Initialize the Bluetooth Subsystem */
	err = bt_enable(bt_ready);
	if (err) {
		printk("Bluetooth init failed (err %d)\n", err);
	}
}
