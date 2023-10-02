
from flask import Flask, jsonify

import ctypes
clibrary = ctypes.CDLL("/home/medino/Documentos/Faculdade/Discreta/comunicarPy/clibrary.so")

app = Flask(__name__)
@app.route('/')

def home():

    return'API Em Funcionamento!'

#Contruir Funcionalidades 
@app.route('/chavepublica')

def gerarChave():
    key = clibrary.chavePublica
    

    key.argtypes = [ctypes.c_char_p, ctypes.c_char_p, ctypes.c_char_p]
    key.restype = ctypes.c_char_p
    response_chave = {'Chave_Publica': f'{key(b"113", b"131", b"163")}'}
    #

    return jsonify(response_chave)
      


@app.route('/encriptar')

def encriptar():
    encript = clibrary.encriptar

    encript.argtypes = [ctypes.c_char_p, ctypes.c_char_p, ctypes.c_char_p]
    encript.restype = ctypes.c_char_p
    response_encriptar = {'Mensagem_Encriptada': f'{encript(b"Josue Medino da Silva ", b"14803", b"163")}'}
    #

    return jsonify(response_encriptar)

@app.route('/desencriptar')
    
def desencriptar():
    desencript = clibrary.desencriptar
   
    desencript.argtypes = [ctypes.c_char_p, ctypes.c_char_p, ctypes.c_char_p, ctypes.c_char_p]
    desencript.restype = ctypes.c_char_p
    response_desencriptar ={ 'Mensagem_Desencriptada': f'{desencript(b"113", b"131", b"163", b"8542 4015 2878 11171 9850 10630 5157 9850 5754 5707 9851 4015 10630 5754 4377 10630 10852 5707 6977 14267 4377 ")}'}
   
    return jsonify(response_desencriptar)

#Rodar api
if __name__ == '__main__':
    app.run(debug=True)