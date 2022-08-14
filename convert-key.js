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
  .map(b => `0x${b}`)

  // Join them by ", " sequence making it valid C-style array definition
  .join(', ');

console.log(keyHex);
