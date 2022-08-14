#include <zephyr/types.h>
#include <stddef.h>
#include <sys/printk.h>
#include <sys/util.h>

#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>

#define DEVICE_NAME CONFIG_BT_DEVICE_NAME
#define DEVICE_NAME_LEN (sizeof(DEVICE_NAME) - 1)

/* Paste your advertisement payload here */

void set_payload_from_key(uint8_t *payload, uint8_t *public_key) {
  memcpy(&payload[5], &public_key[6], 22);
  payload[27] = public_key[0] >> 6;
}

static void bt_ready(int err)
{
	/* Bluetooth initialization error handler */
	if (err) { return; }

	/* Prepare scan response containing Zephyr's Device Name */
	static const struct bt_data scan_response[] = {
		BT_DATA(BT_DATA_NAME_COMPLETE, DEVICE_NAME, DEVICE_NAME_LEN),
	};

	/* Start advertising */
	err = bt_le_adv_start(
		BT_LE_ADV_NCONN_IDENTITY, advertisement_payload, 
		ARRAY_SIZE(advertisement_payload),
		scan_response, 
		ARRAY_SIZE(scan_response)
	);

	/* Advertising error handler */
	if (err) { return; }
}

void main(void)
{
	bt_enable(bt_ready);
}
