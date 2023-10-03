
from flask import Flask, jsonify

import ctypes
clibrary = ctypes.CDLL("/home/medino/Documentos/Faculdade/Discreta/comunicarPy/clibrary.so")

app = Flask(__name__)
@app.route('/')

def home():

    return'API Em Funcionamento!'

#Contruir Funcionalidades 
@app.route('/chavepublica/<string:p>/<string:q>/<string:e>')

def gerarChave(p, q, e):
   

    key = clibrary.chavePublica
    key.argtypes = [ctypes.c_char_p, ctypes.c_char_p, ctypes.c_char_p]
    key.restype = ctypes.c_char_p
    response = key(p.encode(), q.encode(), e.encode()).decode('utf-8')
    
    response_chave = {'Chave_Publica': f'{response}'}
    

    return jsonify(response_chave)
      


@app.route('/encriptar/<string:msg_pura>/<string:n>/<string:e>')

def encriptar(msg_pura, n, e):
    encript = clibrary.encriptar

    encript.argtypes = [ctypes.c_char_p, ctypes.c_char_p, ctypes.c_char_p]
    encript.restype = ctypes.c_char_p
    response_E = encript(msg_pura.encode(), n.encode(), e.encode()).decode('utf-8')
    response_encriptar = {'Mensagem_Encriptada': f'{response_E}'}
    

    return jsonify(response_encriptar)

@app.route('/desencriptar/<string:p>/<string:q>/<string:e>/<string:msg_encriptada>')
    
def desencriptar(p, q, e, msg_encriptada):
    desencript = clibrary.desencriptar
    desencript.argtypes = [ctypes.c_char_p, ctypes.c_char_p, ctypes.c_char_p, ctypes.c_char_p]
    desencript.restype = ctypes.c_char_p
    response_D = desencript(p.encode(), q.encode(), e.encode(), msg_encriptada.encode()).decode('utf-8')
    response_desencriptar ={ 'Mensagem_Desencriptada': f'{response_D}'}
   
    return jsonify(response_desencriptar)

#Rodar api
if __name__ == '__main__':
    app.run(debug=True)