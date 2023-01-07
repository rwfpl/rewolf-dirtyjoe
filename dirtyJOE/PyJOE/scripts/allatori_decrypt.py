################################################################################
#
# Sample string decrypter for dirtyJOE, with proper key it can decrypt strings
# from files obfuscated by Allatori Obfuscator.
#
# Copyright 2011 ReWolf
#
################################################################################

import pyjoe

def dj_decryptUTF8(inBuf):
	key = [1, 2]
	return pyjoe.dj_decryptUTF8_Allatori(inBuf, key)
