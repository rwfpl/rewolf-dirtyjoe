################################################################################
#
# Python helper library for dirtyJOE
#
# Copyright 2011 ReWolf
#
################################################################################


def dj_normalizeUtf8(inBuffer):

	i = 0
	ret = []
	max = len(inBuffer)

	while i < max:
		c = inBuffer[i]
		if c == 0:
			break

		defAdd = 1
		if (c >> 5) == 6:
			c = ((c & 0x1f) << 6) + (inBuffer[i+1] & 0x3f)
			defAdd = 2
		elif (c >> 4) == 0xE:
			c = ((c & 0xf) << 12) + ((inBuffer[i+1] & 0x3f) << 6) + (inBuffer[i+2] & 0x3f)
			defAdd = 3

		ret += [c]
		i += defAdd

	return ret


def dj_encodeUtf8(inBuffer):

	i = 0
	ret = []
	max = len(inBuffer)

	while i < max:
		c = inBuffer[i]
		if c >= 1 and c <= 0x007F:
			ret += [c]
		elif (c == 0) or (c >= 0x80 and c < 0x7FF):
			ret += [0xC0 | (c >> 6)]
			ret += [0x80 | (c & 0x3F)]
		elif c >= 0x800 and c < 0xFFFF:
			ret += [0xE0 | (c >> 12)]
			ret += [0x80 | ((c >> 6) & 0x3F)]
			ret += [0x80 | (c & 0x3F)]
		else:
			break
		i += 1

	return ret


def dj_decryptUTF8_ZKM(inBuffer, key):

	tstr = dj_normalizeUtf8(inBuffer)
	i = 0
	ret = []
	last = len(tstr)
	while i < last:
		ret += [tstr[i] ^ key[i % 5]]
		i += 1

	return dj_encodeUtf8(ret)


def dj_decryptUTF8_Allatori(inBuffer, key):

	tstr = dj_normalizeUtf8(inBuffer)
	last = len(tstr)
	ret = range(last)
	last -= 1
	kk = key[0]
	while last >= 0:
		ret[last] = tstr[last] ^ kk
		kk = key[1] & (kk ^ last)
		last -= 1

	return dj_encodeUtf8(ret)
