$('#myModal').on('shown.bs.modal', function () {
    $('#myInput').trigger('focus')
  });


  $("#sendReceta").click(function(){
   receta = document.getElementById('receta').value;
   const data = new URLSearchParams();
   data.append('receta', receta);
   fetch('./php/sendReceta.php', {
      method: 'POST',
      body: data
   })
   .then(function(response) {
      if(response.ok) {
          return response.text()
      } else {
          throw "Error en la llamada Ajax";
      }
 
   })
   .then(function(texto) {
      if (texto == "Okay") {
         $('#recetacollapseOkay').collapse('show');
         //console.log(texto);
         setTimeout(function(){ $('#recetacollapseOkay').collapse('hide'); }, 2000);
         
      }else{
         $('#recetacollapseFail').collapse('show');
         //console.log(texto);
         setTimeout(function(){ $('#recetacollapseFail').collapse('hide'); }, 2000);
 
      }
   });
 });

 $("#cancelar").click(function(){
   const data = new URLSearchParams();
   data.append('IDplaca', 1);
   fetch('./php/cancelarProceso.php', {
      method: 'POST',
      body: data
   })
   .then(function(response) {
      if(response.ok) {
          return response.text();
      } else {
          throw "Error en la llamada Ajax";
      }
 
   })
   .then(function(texto) {
      console.log(texto);
   })
   .catch(function(err) {
      console.log(err);
   });
 });

 $("#sendProceso").click(function(){
   proceso = document.getElementById('proceso').value;
   paso = document.getElementById('paso').value;
   const data = new URLSearchParams();
   data.append('IDplaca', 1);
   data.append('proceso', proceso);
   data.append('paso', paso);
   fetch('./php/sendProceso.php', {
      method: 'POST',
      body: data
   })
   .then(function(response) {
      if(response.ok) {
          return response.text();
      } else {
          throw "Error en la llamada Ajax";
      }
 
   })
   .then(function(texto) {
      if (texto == "Okay") {
         $('#procesocollapseOkay').collapse('show');
         //console.log(texto);
         setTimeout(function(){ $('#procesocollapseOkay').collapse('hide'); }, 2000);
         
      }else{
         $('#procesocollapseFail').collapse('show');
         //console.log(texto);
         setTimeout(function(){ $('#procesocollapseFail').collapse('hide'); }, 2000);
 
      }
   });
 });
