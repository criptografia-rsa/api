// Fazer uma solicitação GET para a rota /encriptar
fetch('http://127.0.0.1:5000/desencriptar')
  .then(response => response.json())
  .then(data => {
    // Processar a resposta da API
    console.log(data);
  })
  .catch(error => {
    console.error('Erro:', error);
  });
