$('#myModal').on('shown.bs.modal', function () {
    $('#myInput').trigger('focus')
  });


  $("#sendReceta").click(function(){
   idPlaca = document.getElementById('idPlacaReceta').value;
   receta = document.getElementById('receta').value;
   const data = new URLSearchParams();
   data.append('idPlaca', idPlaca);
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
   idPlaca = document.getElementById('idPlacaCancelar').value;
   const data = new URLSearchParams();
   data.append('idPlaca', idPlaca);
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
   idPlaca = document.getElementById('idPlacaProceso').value;
   proceso = document.getElementById('proceso').value;
   paso = document.getElementById('paso').value;
   const data = new URLSearchParams();
   data.append('idPlaca', idPlaca);
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

 $("#sendSettings").click(function(){
   idPlaca = document.getElementById('idPlacaAjustes').value;
   setting = document.getElementById('settings').value;
   const data = new URLSearchParams();
   data.append('idPlaca', idPlaca);
   data.append('setting', setting);
   fetch('./php/sendSettings.php', {
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
         $('#settingcollapseOkay').collapse('show');
         //console.log(texto);
         setTimeout(function(){ $('#settingcollapseOkay').collapse('hide'); }, 2000);
         
      }else{
         $('#settingcollapseFail').collapse('show');
         //console.log(texto);
         setTimeout(function(){ $('#settingcollapseFail').collapse('hide'); }, 2000);
 
      }
   });
 });