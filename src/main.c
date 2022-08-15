#include <zephyr/types.h>
#include <bluetooth/bluetooth.h>

/* Paste your advertisement payload and MAC here */

static void bt_ready(int err)
{
	/* Bluetooth initialization error handler */
	if (err) { return; }

	/* Start advertising */
	err = bt_le_adv_start(
		BT_LE_ADV_NCONN_IDENTITY, advertisement_payload, 
		ARRAY_SIZE(advertisement_payload),
		NULL, // Don't send the second packet with device name
		0
	);

	/* Advertising error handler */
	if (err) { return; }
}

void main(void)
{
	bt_addr_le_t ble_addr = { BT_ADDR_LE_RANDOM, addr };
	bt_id_create(&ble_addr, NULL);
	bt_enable(bt_ready);
}
