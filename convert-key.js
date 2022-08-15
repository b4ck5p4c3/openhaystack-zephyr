#!/usr/bin/env node

const keyString = process.argv[2];
if (!keyString)
  throw new Error('Key is empty');

const key = Buffer.from(keyString, 'base64');
if (key.length !== 28)
  throw new Error(`Incorrect key size: decoded ${key.length} bytes, but 28 bytes expected`);

const keyHex = key
  // Convert into hex
  .toString('hex')

  // Split hex string into single byte length string (two char chunks)
  .match(/.{1,2}/g)

  // Append "0x" designator to each chunk
  .map(b => `0x${b}`);

// Find My BLE advertisement payload template
const snippet = `
static const struct bt_data advertisement_payload[] = {
  BT_DATA_BYTES(BT_DATA_MANUFACTURER_DATA,
  0x4c, 0x00, 0x12, 0x19, 0x00,
  ${keyHex.slice(6, 14).join(', ')},
  ${keyHex.slice(14, 22).join(', ')},
  ${keyHex.slice(22, 28).join(', ')},
  0x${(key[0] >> 6).toString(16)}, 0x00)
};
`;

console.log(snippet);
