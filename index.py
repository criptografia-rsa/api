import ctypes

clibrary = ctypes.CDLL("/home/medino/Documentos/Faculdade/Discreta/comunicarPy/clibrary.so")

#clibrary.display()

key = clibrary.chavePublica

key.argtypes = [ctypes.c_char_p, ctypes.c_char_p, ctypes.c_char_p]
key.restype = ctypes.c_char_p
response = key(b"113", b"131", b"163")


encript = clibrary.encriptar

encript.argtypes = [ctypes.c_char_p, ctypes.c_char_p, ctypes.c_char_p]
encript.restype = ctypes.c_char_p
response_encript = encript(b"Josue Medino da Silva ", b"14803", b"163")


desencript = clibrary.desencriptar

desencript.argtypes = [ctypes.c_char_p, ctypes.c_char_p, ctypes.c_char_p, ctypes.c_char_p]
desencript.restype = ctypes.c_char_p
response_desencript = desencript(b"113", b"131", b"163", b"8542 4015 2878 11171 9850 10630 5157 9850 5754 5707 9851 4015 10630 5754 4377 10630 10852 5707 6977 14267 4377")



print(response)
print(response_encript)
print(response_desencript)
