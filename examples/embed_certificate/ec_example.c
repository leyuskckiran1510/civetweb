﻿#include <stdio.h>
#include <openssl/ssl.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>

#include "civetweb.h"

/* server.crt BASE64 to HEX */
const uint8_t SSL_CERT_ASN1[] = {
    0x30, 0x82, 0x04, 0x40, 0x30, 0x82, 0x03, 0x28, 0xa0, 0x03, 0x02, 0x01,
    0x02, 0x02, 0x14, 0x49, 0x65, 0x5b, 0x35, 0xce, 0x42, 0x20, 0x15, 0xa7,
    0xc4, 0x8a, 0x29, 0xe6, 0x44, 0x58, 0x6c, 0x11, 0xe6, 0x8c, 0xa7, 0x30,
    0x0d, 0x06, 0x09, 0x2a, 0x86, 0x48, 0x86, 0xf7, 0x0d, 0x01, 0x01, 0x0b,
    0x05, 0x00, 0x30, 0x71, 0x31, 0x0b, 0x30, 0x09, 0x06, 0x03, 0x55, 0x04,
    0x06, 0x13, 0x02, 0x41, 0x41, 0x31, 0x12, 0x30, 0x10, 0x06, 0x03, 0x55,
    0x04, 0x08, 0x0c, 0x09, 0x6c, 0x6f, 0x63, 0x61, 0x6c, 0x68, 0x6f, 0x73,
    0x74, 0x31, 0x12, 0x30, 0x10, 0x06, 0x03, 0x55, 0x04, 0x07, 0x0c, 0x09,
    0x6c, 0x6f, 0x63, 0x61, 0x6c, 0x68, 0x6f, 0x73, 0x74, 0x31, 0x12, 0x30,
    0x10, 0x06, 0x03, 0x55, 0x04, 0x0a, 0x0c, 0x09, 0x6c, 0x6f, 0x63, 0x61,
    0x6c, 0x68, 0x6f, 0x73, 0x74, 0x31, 0x12, 0x30, 0x10, 0x06, 0x03, 0x55,
    0x04, 0x0b, 0x0c, 0x09, 0x6c, 0x6f, 0x63, 0x61, 0x6c, 0x68, 0x6f, 0x73,
    0x74, 0x31, 0x12, 0x30, 0x10, 0x06, 0x03, 0x55, 0x04, 0x03, 0x0c, 0x09,
    0x6c, 0x6f, 0x63, 0x61, 0x6c, 0x68, 0x6f, 0x73, 0x74, 0x30, 0x1e, 0x17,
    0x0d, 0x32, 0x31, 0x30, 0x34, 0x30, 0x34, 0x31, 0x38, 0x33, 0x39, 0x35,
    0x34, 0x5a, 0x17, 0x0d, 0x33, 0x31, 0x30, 0x34, 0x30, 0x32, 0x31, 0x38,
    0x33, 0x39, 0x35, 0x34, 0x5a, 0x30, 0x71, 0x31, 0x0b, 0x30, 0x09, 0x06,
    0x03, 0x55, 0x04, 0x06, 0x13, 0x02, 0x41, 0x41, 0x31, 0x12, 0x30, 0x10,
    0x06, 0x03, 0x55, 0x04, 0x08, 0x0c, 0x09, 0x6c, 0x6f, 0x63, 0x61, 0x6c,
    0x68, 0x6f, 0x73, 0x74, 0x31, 0x12, 0x30, 0x10, 0x06, 0x03, 0x55, 0x04,
    0x07, 0x0c, 0x09, 0x6c, 0x6f, 0x63, 0x61, 0x6c, 0x68, 0x6f, 0x73, 0x74,
    0x31, 0x12, 0x30, 0x10, 0x06, 0x03, 0x55, 0x04, 0x0a, 0x0c, 0x09, 0x6c,
    0x6f, 0x63, 0x61, 0x6c, 0x68, 0x6f, 0x73, 0x74, 0x31, 0x12, 0x30, 0x10,
    0x06, 0x03, 0x55, 0x04, 0x0b, 0x0c, 0x09, 0x6c, 0x6f, 0x63, 0x61, 0x6c,
    0x68, 0x6f, 0x73, 0x74, 0x31, 0x12, 0x30, 0x10, 0x06, 0x03, 0x55, 0x04,
    0x03, 0x0c, 0x09, 0x6c, 0x6f, 0x63, 0x61, 0x6c, 0x68, 0x6f, 0x73, 0x74,
    0x30, 0x82, 0x01, 0x22, 0x30, 0x0d, 0x06, 0x09, 0x2a, 0x86, 0x48, 0x86,
    0xf7, 0x0d, 0x01, 0x01, 0x01, 0x05, 0x00, 0x03, 0x82, 0x01, 0x0f, 0x00,
    0x30, 0x82, 0x01, 0x0a, 0x02, 0x82, 0x01, 0x01, 0x00, 0xca, 0x25, 0x51,
    0xe7, 0xb4, 0x06, 0x2d, 0x11, 0xe1, 0x33, 0xfb, 0xde, 0xe1, 0x92, 0xbe,
    0x63, 0x29, 0xf9, 0x1a, 0xea, 0xcd, 0x2d, 0x71, 0xf8, 0xc9, 0xae, 0x2b,
    0x35, 0xae, 0x6f, 0xd1, 0x71, 0xae, 0xf5, 0x6f, 0xcc, 0xdd, 0xcf, 0x25,
    0xd1, 0x59, 0x78, 0x30, 0x82, 0xec, 0x65, 0xe3, 0xd7, 0xa2, 0x6f, 0x7c,
    0x84, 0x82, 0xec, 0x2f, 0xd4, 0x37, 0x7d, 0xbc, 0x6c, 0x36, 0x07, 0x82,
    0x24, 0xed, 0x21, 0xe5, 0x81, 0xc3, 0x12, 0x2b, 0x54, 0x96, 0x44, 0xf7,
    0xdd, 0x6a, 0x58, 0xe1, 0x2f, 0x3f, 0xcd, 0x67, 0x1e, 0x32, 0xc8, 0x99,
    0x9b, 0xda, 0x33, 0x54, 0xdb, 0x52, 0x5d, 0xb2, 0xc5, 0x80, 0x50, 0x3b,
    0x41, 0xe1, 0xd3, 0xa8, 0x0c, 0x0d, 0x23, 0x90, 0xe4, 0x62, 0xf4, 0x4f,
    0xd6, 0x1b, 0xc5, 0x5a, 0xe0, 0x18, 0xa9, 0x3d, 0x74, 0xbb, 0x30, 0x20,
    0xbd, 0x85, 0xbd, 0xc3, 0xbd, 0x69, 0x86, 0x62, 0x75, 0xa6, 0x9e, 0x1c,
    0x74, 0x45, 0xa2, 0x5e, 0x67, 0x4c, 0xe0, 0xa2, 0x87, 0x13, 0x28, 0x8c,
    0x76, 0xbf, 0x2e, 0xdf, 0xe7, 0xaf, 0x79, 0x12, 0xa9, 0x65, 0x42, 0x66,
    0x1c, 0x25, 0xee, 0xca, 0x45, 0x29, 0x30, 0xff, 0x9c, 0x26, 0x14, 0x06,
    0x63, 0x91, 0x08, 0xa4, 0x58, 0x8d, 0x22, 0x0b, 0xc8, 0x18, 0x8e, 0x42,
    0xcd, 0x16, 0xb5, 0xdf, 0xa8, 0x7b, 0x7a, 0xb4, 0x71, 0x5f, 0xc2, 0xaa,
    0x60, 0x99, 0xc5, 0x2f, 0xef, 0x13, 0xd1, 0x9b, 0x9c, 0x0a, 0x8c, 0x14,
    0x7a, 0x8a, 0xb9, 0xdf, 0xdf, 0x9d, 0xaa, 0x76, 0x52, 0xff, 0x0c, 0x93,
    0x68, 0x3f, 0x73, 0x0d, 0xb7, 0xc0, 0x21, 0x67, 0x48, 0xfd, 0xf2, 0xe2,
    0xc3, 0x9c, 0xf5, 0xd4, 0x89, 0x86, 0xa9, 0x7b, 0x6a, 0x1b, 0x81, 0xa0,
    0x89, 0x39, 0x71, 0xc9, 0x3a, 0x65, 0x54, 0xdb, 0x06, 0x22, 0x82, 0x7f,
    0xc7, 0x02, 0x03, 0x01, 0x00, 0x01, 0xa3, 0x81, 0xcf, 0x30, 0x81, 0xcc,
    0x30, 0x81, 0x98, 0x06, 0x03, 0x55, 0x1d, 0x23, 0x04, 0x81, 0x90, 0x30,
    0x81, 0x8d, 0xa1, 0x75, 0xa4, 0x73, 0x30, 0x71, 0x31, 0x0b, 0x30, 0x09,
    0x06, 0x03, 0x55, 0x04, 0x06, 0x13, 0x02, 0x41, 0x41, 0x31, 0x12, 0x30,
    0x10, 0x06, 0x03, 0x55, 0x04, 0x08, 0x0c, 0x09, 0x6c, 0x6f, 0x63, 0x61,
    0x6c, 0x68, 0x6f, 0x73, 0x74, 0x31, 0x12, 0x30, 0x10, 0x06, 0x03, 0x55,
    0x04, 0x07, 0x0c, 0x09, 0x6c, 0x6f, 0x63, 0x61, 0x6c, 0x68, 0x6f, 0x73,
    0x74, 0x31, 0x12, 0x30, 0x10, 0x06, 0x03, 0x55, 0x04, 0x0a, 0x0c, 0x09,
    0x6c, 0x6f, 0x63, 0x61, 0x6c, 0x68, 0x6f, 0x73, 0x74, 0x31, 0x12, 0x30,
    0x10, 0x06, 0x03, 0x55, 0x04, 0x0b, 0x0c, 0x09, 0x6c, 0x6f, 0x63, 0x61,
    0x6c, 0x68, 0x6f, 0x73, 0x74, 0x31, 0x12, 0x30, 0x10, 0x06, 0x03, 0x55,
    0x04, 0x03, 0x0c, 0x09, 0x6c, 0x6f, 0x63, 0x61, 0x6c, 0x68, 0x6f, 0x73,
    0x74, 0x82, 0x14, 0x60, 0x3a, 0x75, 0xa0, 0xfc, 0x63, 0x14, 0xe1, 0x70,
    0x07, 0x16, 0x23, 0x7b, 0x84, 0x63, 0x3d, 0xe1, 0x1d, 0x07, 0x12, 0x30,
    0x0c, 0x06, 0x03, 0x55, 0x1d, 0x13, 0x01, 0x01, 0xff, 0x04, 0x02, 0x30,
    0x00, 0x30, 0x0b, 0x06, 0x03, 0x55, 0x1d, 0x0f, 0x04, 0x04, 0x03, 0x02,
    0x04, 0xf0, 0x30, 0x14, 0x06, 0x03, 0x55, 0x1d, 0x11, 0x04, 0x0d, 0x30,
    0x0b, 0x82, 0x09, 0x6c, 0x6f, 0x63, 0x61, 0x6c, 0x68, 0x6f, 0x73, 0x74,
    0x30, 0x0d, 0x06, 0x09, 0x2a, 0x86, 0x48, 0x86, 0xf7, 0x0d, 0x01, 0x01,
    0x0b, 0x05, 0x00, 0x03, 0x82, 0x01, 0x01, 0x00, 0x13, 0x3c, 0x41, 0x03,
    0xb2, 0x58, 0x00, 0x0a, 0x73, 0x1f, 0xb4, 0x15, 0xc7, 0x5f, 0xa5, 0x07,
    0x14, 0xa6, 0xee, 0x7c, 0x83, 0x40, 0x85, 0x70, 0x40, 0x6c, 0x08, 0x94,
    0x71, 0x77, 0x1c, 0xd8, 0x9a, 0xb3, 0x8d, 0x7c, 0xfa, 0x45, 0x96, 0x03,
    0x51, 0x92, 0x46, 0xf8, 0x84, 0x6d, 0x09, 0x40, 0x3a, 0x53, 0x07, 0xc3,
    0xd6, 0x1f, 0x72, 0xe6, 0x15, 0xeb, 0x49, 0x3a, 0x63, 0x2d, 0x11, 0x58,
    0x73, 0xe3, 0x25, 0xde, 0xb0, 0x85, 0x5b, 0x45, 0xc6, 0xb9, 0x46, 0x2d,
    0x86, 0x2d, 0xc9, 0x9b, 0x1f, 0x73, 0x6a, 0x18, 0xe7, 0x94, 0x49, 0x4c,
    0x77, 0xea, 0x0b, 0xb6, 0xd1, 0xfb, 0x77, 0x6a, 0x1b, 0xfc, 0xb0, 0xcf,
    0x5e, 0x29, 0xf1, 0xd5, 0xfc, 0x93, 0x8f, 0x48, 0xe3, 0xe0, 0xbd, 0x09,
    0xa5, 0x7d, 0xa1, 0x50, 0x2c, 0x9f, 0xdc, 0x44, 0xe4, 0x85, 0xbb, 0xca,
    0xab, 0xbd, 0x73, 0xa7, 0xd8, 0x3e, 0x31, 0x53, 0xfe, 0xd4, 0xc3, 0xd7,
    0x05, 0x3c, 0x93, 0x72, 0xcc, 0x16, 0x2b, 0xe6, 0x33, 0xf0, 0xbd, 0xea,
    0x9a, 0x5f, 0x24, 0x39, 0x7e, 0xfe, 0x82, 0x34, 0xa6, 0xb2, 0x23, 0xe4,
    0x67, 0x8f, 0xf9, 0x60, 0x6b, 0xd7, 0x2e, 0xd9, 0xfb, 0x9a, 0x12, 0x3c,
    0xb5, 0x3b, 0xf9, 0x9d, 0x0a, 0xbf, 0xff, 0x2d, 0x6b, 0x33, 0x82, 0xe6,
    0x54, 0x75, 0x2b, 0xf2, 0x74, 0x3b, 0xc7, 0x96, 0xa5, 0x06, 0x70, 0xb6,
    0x0a, 0x43, 0x7a, 0x7b, 0xa1, 0xc3, 0x8e, 0x2a, 0x58, 0xcc, 0xff, 0x49,
    0xeb, 0x51, 0xb6, 0x7c, 0x7e, 0xa2, 0x68, 0x9f, 0x82, 0xe8, 0x2e, 0x4e,
    0xf9, 0x14, 0xc7, 0xbe, 0x60, 0x42, 0x65, 0x9d, 0x5a, 0xce, 0xca, 0x09,
    0xf7, 0xee, 0x48, 0x41, 0x93, 0x69, 0x7c, 0x3e, 0x47, 0xf0, 0x5e, 0x60,
    0x5d, 0x1d, 0xa3, 0x67, 0x59, 0x3c, 0xf9, 0x3c, 0x49, 0x63, 0x74, 0x84};

/* server.key BASE64 to HEX */
const uint8_t SSL_KEY_ASN1[] = {
    0x30, 0x82, 0x04, 0xa2, 0x02, 0x01, 0x00, 0x02, 0x82, 0x01, 0x01, 0x00,
    0xca, 0x25, 0x51, 0xe7, 0xb4, 0x06, 0x2d, 0x11, 0xe1, 0x33, 0xfb, 0xde,
    0xe1, 0x92, 0xbe, 0x63, 0x29, 0xf9, 0x1a, 0xea, 0xcd, 0x2d, 0x71, 0xf8,
    0xc9, 0xae, 0x2b, 0x35, 0xae, 0x6f, 0xd1, 0x71, 0xae, 0xf5, 0x6f, 0xcc,
    0xdd, 0xcf, 0x25, 0xd1, 0x59, 0x78, 0x30, 0x82, 0xec, 0x65, 0xe3, 0xd7,
    0xa2, 0x6f, 0x7c, 0x84, 0x82, 0xec, 0x2f, 0xd4, 0x37, 0x7d, 0xbc, 0x6c,
    0x36, 0x07, 0x82, 0x24, 0xed, 0x21, 0xe5, 0x81, 0xc3, 0x12, 0x2b, 0x54,
    0x96, 0x44, 0xf7, 0xdd, 0x6a, 0x58, 0xe1, 0x2f, 0x3f, 0xcd, 0x67, 0x1e,
    0x32, 0xc8, 0x99, 0x9b, 0xda, 0x33, 0x54, 0xdb, 0x52, 0x5d, 0xb2, 0xc5,
    0x80, 0x50, 0x3b, 0x41, 0xe1, 0xd3, 0xa8, 0x0c, 0x0d, 0x23, 0x90, 0xe4,
    0x62, 0xf4, 0x4f, 0xd6, 0x1b, 0xc5, 0x5a, 0xe0, 0x18, 0xa9, 0x3d, 0x74,
    0xbb, 0x30, 0x20, 0xbd, 0x85, 0xbd, 0xc3, 0xbd, 0x69, 0x86, 0x62, 0x75,
    0xa6, 0x9e, 0x1c, 0x74, 0x45, 0xa2, 0x5e, 0x67, 0x4c, 0xe0, 0xa2, 0x87,
    0x13, 0x28, 0x8c, 0x76, 0xbf, 0x2e, 0xdf, 0xe7, 0xaf, 0x79, 0x12, 0xa9,
    0x65, 0x42, 0x66, 0x1c, 0x25, 0xee, 0xca, 0x45, 0x29, 0x30, 0xff, 0x9c,
    0x26, 0x14, 0x06, 0x63, 0x91, 0x08, 0xa4, 0x58, 0x8d, 0x22, 0x0b, 0xc8,
    0x18, 0x8e, 0x42, 0xcd, 0x16, 0xb5, 0xdf, 0xa8, 0x7b, 0x7a, 0xb4, 0x71,
    0x5f, 0xc2, 0xaa, 0x60, 0x99, 0xc5, 0x2f, 0xef, 0x13, 0xd1, 0x9b, 0x9c,
    0x0a, 0x8c, 0x14, 0x7a, 0x8a, 0xb9, 0xdf, 0xdf, 0x9d, 0xaa, 0x76, 0x52,
    0xff, 0x0c, 0x93, 0x68, 0x3f, 0x73, 0x0d, 0xb7, 0xc0, 0x21, 0x67, 0x48,
    0xfd, 0xf2, 0xe2, 0xc3, 0x9c, 0xf5, 0xd4, 0x89, 0x86, 0xa9, 0x7b, 0x6a,
    0x1b, 0x81, 0xa0, 0x89, 0x39, 0x71, 0xc9, 0x3a, 0x65, 0x54, 0xdb, 0x06,
    0x22, 0x82, 0x7f, 0xc7, 0x02, 0x03, 0x01, 0x00, 0x01, 0x02, 0x82, 0x01,
    0x00, 0x73, 0xbb, 0x54, 0x1e, 0x34, 0xca, 0x48, 0x69, 0x71, 0x26, 0xc2,
    0xf0, 0x02, 0xf3, 0x71, 0xbe, 0xf2, 0x5b, 0xe5, 0x16, 0x42, 0xeb, 0xde,
    0xd1, 0x92, 0x1d, 0xfe, 0x2d, 0x18, 0xb6, 0x7a, 0x11, 0xfd, 0x1a, 0x15,
    0xad, 0x13, 0xdc, 0xb2, 0x09, 0x1e, 0x91, 0x1a, 0x2d, 0x0a, 0xcc, 0xf6,
    0xda, 0x10, 0xec, 0x85, 0x3c, 0x94, 0x7c, 0x46, 0x91, 0xd8, 0x47, 0x4b,
    0x66, 0x24, 0xb4, 0xbd, 0xc5, 0x08, 0x62, 0x9c, 0xb4, 0x63, 0x0b, 0x76,
    0xf5, 0x51, 0xa7, 0x20, 0xc5, 0x8a, 0x4a, 0x62, 0x7a, 0x1b, 0xac, 0x2c,
    0x7a, 0x74, 0x96, 0xb6, 0xa3, 0x2d, 0x14, 0xb0, 0x63, 0x74, 0xcf, 0xa2,
    0x37, 0x42, 0xd4, 0x2c, 0x68, 0xf6, 0xb2, 0xa8, 0x06, 0x66, 0x4b, 0x53,
    0x7b, 0xfe, 0x4f, 0x63, 0x99, 0xf0, 0x82, 0x58, 0x19, 0xee, 0xe4, 0x8e,
    0x03, 0xd3, 0xdb, 0xa5, 0x12, 0xfc, 0x8b, 0xfd, 0x90, 0xe2, 0x55, 0xd8,
    0xb4, 0x59, 0xc8, 0x75, 0xab, 0x14, 0x02, 0x65, 0xee, 0x00, 0xf0, 0xdc,
    0xa4, 0x5f, 0x1e, 0xbb, 0x10, 0x90, 0x57, 0xea, 0x45, 0x33, 0x75, 0xfd,
    0x0c, 0x97, 0xe6, 0xd6, 0xe6, 0xaa, 0x08, 0x22, 0x26, 0x4a, 0x34, 0x0d,
    0xfe, 0xb5, 0xde, 0xdb, 0xa8, 0xc0, 0x67, 0x83, 0x5f, 0x27, 0x62, 0x55,
    0x5a, 0xa0, 0x83, 0x25, 0xe6, 0x19, 0xd9, 0x78, 0x33, 0x7b, 0x2b, 0xa0,
    0x53, 0x1d, 0xed, 0x62, 0xd1, 0x10, 0x95, 0x9f, 0xb0, 0xa2, 0xe7, 0xe8,
    0x58, 0x39, 0x31, 0x76, 0x65, 0x53, 0x28, 0x23, 0x98, 0xa9, 0xab, 0xc2,
    0x9c, 0x57, 0x0e, 0x9c, 0x17, 0x10, 0x14, 0x35, 0x5b, 0x5d, 0xce, 0x94,
    0x1b, 0xdf, 0x8b, 0x42, 0xc0, 0xc6, 0x0e, 0x5c, 0x48, 0x67, 0x4f, 0xaf,
    0x27, 0x3d, 0xc4, 0xda, 0xfa, 0xb4, 0xbd, 0x8e, 0x55, 0xdd, 0xa8, 0x18,
    0x34, 0x01, 0xb6, 0xd8, 0x09, 0x02, 0x81, 0x81, 0x00, 0xe9, 0x53, 0x5f,
    0xf9, 0x0e, 0xdf, 0x1e, 0x18, 0x90, 0xcb, 0xb8, 0x66, 0xea, 0x70, 0x6a,
    0x72, 0xc3, 0x6e, 0x87, 0x7a, 0x79, 0x2f, 0xc4, 0x50, 0x32, 0x32, 0xcf,
    0x97, 0xa7, 0x3b, 0x2b, 0x90, 0xd1, 0x05, 0x39, 0x5f, 0x51, 0x47, 0x79,
    0xc3, 0x1d, 0xd8, 0xaa, 0xca, 0xda, 0x1a, 0x38, 0x6b, 0xfa, 0x02, 0x29,
    0xbd, 0x43, 0x45, 0xed, 0xe8, 0xae, 0xf5, 0xc0, 0xaa, 0xde, 0xe8, 0xac,
    0x21, 0xf5, 0x62, 0x99, 0xea, 0xeb, 0xb2, 0x54, 0xfd, 0xf3, 0x6a, 0x9d,
    0x13, 0xbe, 0x09, 0x51, 0xef, 0x0f, 0x12, 0xb2, 0x14, 0x20, 0xe9, 0x6d,
    0xfe, 0x6c, 0x63, 0x02, 0x7c, 0xd7, 0x0e, 0xa9, 0x2f, 0x2b, 0x52, 0x68,
    0x83, 0x50, 0xdd, 0xc2, 0xf1, 0x86, 0x7c, 0x33, 0xe8, 0x62, 0x6e, 0x8e,
    0x48, 0x50, 0x5c, 0x84, 0x7e, 0x22, 0x36, 0x60, 0x74, 0x16, 0x27, 0xd5,
    0x77, 0xb6, 0x94, 0x7e, 0x75, 0x02, 0x81, 0x81, 0x00, 0xdd, 0xca, 0x42,
    0x1f, 0x3d, 0x3f, 0xc7, 0x4e, 0xce, 0x7d, 0x37, 0x09, 0xef, 0xf8, 0xee,
    0x67, 0x97, 0xbb, 0xf8, 0x34, 0x49, 0x44, 0xa9, 0x9a, 0x07, 0x7f, 0x48,
    0xaa, 0xb9, 0x77, 0xb6, 0x22, 0xfd, 0x88, 0x97, 0x77, 0x20, 0x6e, 0x0c,
    0x67, 0x19, 0x2e, 0xc9, 0x58, 0x3c, 0xfd, 0xdb, 0x3b, 0xfb, 0x0b, 0xfb,
    0x86, 0xa2, 0x74, 0x31, 0x60, 0xaa, 0x27, 0x41, 0x3d, 0xdf, 0x9a, 0xaa,
    0xb3, 0xd8, 0x9a, 0x0a, 0x2d, 0xf9, 0xd7, 0xee, 0x67, 0xdc, 0x49, 0x40,
    0x74, 0x30, 0x32, 0xb7, 0x94, 0xfd, 0x84, 0x13, 0xb8, 0x24, 0x89, 0xdf,
    0xee, 0x7d, 0xe3, 0x1b, 0xe5, 0x76, 0xc4, 0x1b, 0x81, 0x32, 0xa6, 0x0f,
    0x07, 0x26, 0x87, 0x3b, 0xff, 0xaf, 0xa9, 0x25, 0x71, 0xd0, 0x70, 0x2e,
    0xa8, 0xbc, 0x7e, 0xe2, 0xe2, 0x6f, 0x71, 0x5e, 0xe2, 0xad, 0xc1, 0x22,
    0x0c, 0x3f, 0xc4, 0x35, 0xcb, 0x02, 0x81, 0x80, 0x46, 0x40, 0x08, 0x21,
    0x60, 0xcc, 0xe4, 0xae, 0xd8, 0xc9, 0xbd, 0x97, 0x9e, 0xf6, 0x81, 0xd6,
    0x53, 0xe9, 0x2f, 0x79, 0x3c, 0x8b, 0x99, 0x3b, 0xdc, 0x21, 0x58, 0x47,
    0x7c, 0xde, 0x5f, 0xdb, 0x96, 0x53, 0x50, 0x56, 0xd6, 0x8e, 0x02, 0xa7,
    0x30, 0x91, 0x4f, 0xbb, 0x0b, 0xb7, 0xe1, 0x4d, 0x01, 0x55, 0x2d, 0x64,
    0x02, 0xa1, 0x47, 0x64, 0x4b, 0x69, 0x4a, 0xbd, 0x27, 0xa8, 0x3e, 0x4b,
    0x6b, 0x2a, 0x68, 0xd5, 0x46, 0x69, 0xc7, 0x15, 0x3e, 0xf8, 0xd6, 0x9a,
    0x5f, 0x19, 0x47, 0x46, 0x06, 0xef, 0xc6, 0x16, 0x31, 0x62, 0x96, 0xef,
    0x87, 0x8a, 0xb7, 0xf1, 0x06, 0x7f, 0x2f, 0x89, 0x38, 0x2d, 0xf3, 0xb1,
    0xb5, 0xe3, 0x4f, 0x12, 0x91, 0x3f, 0x4c, 0x11, 0xa7, 0xb1, 0x49, 0xbd,
    0x94, 0x14, 0x86, 0xff, 0xc3, 0x25, 0x44, 0x1d, 0x2f, 0x9e, 0x86, 0xb3,
    0x28, 0x91, 0xc5, 0x11, 0x02, 0x81, 0x80, 0x57, 0x5c, 0xef, 0x54, 0xcc,
    0xd4, 0x8d, 0x96, 0x9e, 0x41, 0xb6, 0x67, 0x64, 0xae, 0x62, 0x82, 0x4d,
    0xc3, 0x8e, 0x0e, 0x52, 0x7a, 0x08, 0x70, 0x92, 0xd9, 0x71, 0x6f, 0x46,
    0x65, 0x40, 0x4a, 0x62, 0x21, 0xe6, 0xbf, 0xd6, 0xf7, 0x62, 0x4d, 0x4e,
    0x1f, 0x1e, 0xd2, 0x72, 0x1b, 0xf0, 0xba, 0x9c, 0xb5, 0xe8, 0x9a, 0xec,
    0xec, 0xe5, 0xf2, 0x54, 0xb3, 0xe7, 0xc0, 0x0e, 0x8f, 0x27, 0x04, 0x76,
    0xa2, 0x9e, 0xb5, 0xe3, 0x7f, 0x49, 0xfa, 0x81, 0x4c, 0x1d, 0x66, 0x67,
    0x01, 0xe3, 0x4c, 0x7d, 0xdc, 0x03, 0xc4, 0x7a, 0x28, 0x11, 0x1c, 0x29,
    0x5c, 0x47, 0x45, 0xc8, 0xd5, 0x90, 0x9c, 0x00, 0xae, 0x66, 0xa7, 0x03,
    0x67, 0x2b, 0x9c, 0x18, 0xbe, 0x80, 0xf0, 0x67, 0x11, 0x79, 0x5f, 0x9f,
    0xf8, 0x3f, 0x38, 0xc0, 0x7b, 0x20, 0xcc, 0x1b, 0x73, 0x43, 0x0d, 0x1e,
    0x25, 0x14, 0xa7, 0x02, 0x81, 0x80, 0x09, 0xe0, 0xa5, 0xb9, 0x56, 0x96,
    0x64, 0xf7, 0xd3, 0xc5, 0xbc, 0x19, 0x2b, 0x20, 0xcb, 0x08, 0x9e, 0x0b,
    0x3e, 0x5e, 0xcc, 0x8e, 0xf2, 0x72, 0x83, 0x9b, 0x9f, 0x52, 0xf4, 0x0a,
    0xda, 0xe0, 0x0b, 0x91, 0x14, 0x51, 0x8e, 0x19, 0x1b, 0x77, 0x7d, 0x2a,
    0xec, 0x9a, 0xdc, 0xd1, 0x83, 0xff, 0x25, 0x75, 0xb6, 0xb7, 0xe4, 0x51,
    0xb0, 0xa1, 0x22, 0x7b, 0x1f, 0xb6, 0xcd, 0x7d, 0xe0, 0x55, 0x2f, 0x3d,
    0xe4, 0x1e, 0xe9, 0x4e, 0x77, 0x2e, 0xe5, 0x8c, 0xcb, 0x82, 0x5e, 0xee,
    0xe7, 0x4e, 0x08, 0x09, 0x67, 0xb1, 0xcc, 0x67, 0x66, 0x68, 0xd1, 0x2c,
    0x65, 0x08, 0xc7, 0x8a, 0x23, 0xc4, 0x5b, 0x7c, 0x9c, 0x54, 0x44, 0x2e,
    0xe1, 0xad, 0x8f, 0x99, 0x2a, 0xd8, 0x8f, 0x31, 0x05, 0x7c, 0x9f, 0xbc,
    0x79, 0x67, 0x40, 0x5e, 0xda, 0x2c, 0x38, 0x7d, 0x3f, 0x0b, 0x6c, 0x83,
    0xcc, 0x75};


static int
init_ssl(void *ssl_ctx, void *user_data)
{
	SSL_CTX *ctx = (SSL_CTX *)ssl_ctx;

	SSL_CTX_use_certificate_ASN1(ctx, sizeof(SSL_CERT_ASN1), SSL_CERT_ASN1);
	SSL_CTX_use_PrivateKey_ASN1(EVP_PKEY_RSA,
	                            ctx,
	                            SSL_KEY_ASN1,
	                            sizeof(SSL_KEY_ASN1));

	if (SSL_CTX_check_private_key(ctx) == 0) {
		printf("SSL data inconsistency detected\n");
		return -1;
	}

	return 0; /* let CivetWeb set up the rest of OpenSSL */
}


static int
request_handler(struct mg_connection *conn, void *cbdata)
{
	const struct mg_request_info *ri = mg_get_request_info(conn);
	unsigned uri_len = (unsigned)strlen(ri->local_uri);

	const char *cookie = mg_get_header(conn, "Cookie");
	unsigned long cookie_number = 0;
	if (cookie) {
		char dst[32] = {0};
		if (mg_get_cookie(cookie, "c", dst, sizeof(dst)) > 0) {
			long val = atol(dst);
			if (val > 0) {
				cookie_number = val;
			}
		}
	}
	char new_cookie[32];
	sprintf(new_cookie, "c=%u", cookie_number + 1);

	int status = 200;
	char response[1024];

	if (uri_len <= 100) {
		sprintf(response,
		        "Hello at %s\nYou have visited %u sites here before!",
		        ri->local_uri,
		        cookie_number);
	} else {
		status = 404;
		sprintf(response, "No such URL\n");
	}

	unsigned content_len = (unsigned)strlen(response);
	char content_len_text[32];
	sprintf(content_len_text, "%u", content_len);

	mg_response_header_start(conn, status);
	mg_response_header_add(conn, "Set-Cookie", new_cookie, -1);
	mg_response_header_add(conn,
	                       "Content-Type",
	                       "text/plain; charset=utf-8",
	                       -1);
	mg_response_header_add(conn, "Content-Length", content_len_text, -1);
	mg_response_header_send(conn);
	mg_write(conn, response, content_len);

	return status;
}


int
main(int argc, char *argv[])
{
	mg_init_library(MG_FEATURES_TLS);

	printf("Starting example server at https://localhost:443/\n");
	struct mg_init_data init = {0};

	struct mg_callbacks callbacks = {0};
	callbacks.init_ssl = init_ssl;
	init.callbacks = &callbacks;

	init.user_data = NULL;

	const char *options[] = {"listening_ports", "80r,443s",
	                         "authentication_domain", "localhost",
	                         "enable_auth_domain_check", "no",
	                         "ssl_protocol_version", "4",
	                         "ssl_cipher_list", "ECDH+AESGCM+AES256:!aNULL:!MD5:!DSS",
                             "strict_transport_security_max_age", "15552000",
	                         NULL, NULL}
                             ;
	init.configuration_options = options;

	struct mg_error_data error = {0};
	unsigned error_code = 0;
	error.code = &error_code;
	char error_text[256] = {0};
	error.text = error_text;
	error.text_buffer_size = sizeof(error_text);

	struct mg_context *ctx = mg_start2(&init, &error);
	if (ctx) {
		mg_set_request_handler(ctx, "/", request_handler, NULL);
		printf("Server running for 60 seconds\n");
		sleep(60);
		printf("Server exit\n");
		mg_stop(ctx);
	} else {
		printf("Initialization failed: %u\n%s\n\n", error_code, error_text);
	}

	mg_exit_library();
	return 0;
}
