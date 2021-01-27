<?php
  require './php/conexion.php';
?>
<!DOCTYPE html>
<html lang="es">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <link rel="stylesheet" href="https://maxcdn.bootstrapcdn.com/bootstrap/4.0.0/css/bootstrap.min.css" integrity="sha384-Gn5384xqQ1aoWXA+058RXPxPg6fy4IWvTNh0E263XmFcJlSAwiGgFAW/dAiS6JXm" crossorigin="anonymous">
    <link rel="stylesheet" href="./fontawesome/css/all.min.css">
    <title>Cervecero 2.0</title>
</head>
<body>
    <div class="container">
      
      <nav class="nav nav-pills nav-justified">
        <a class="nav-item nav-link" href="index.html">Main</a>
        <a class="nav-item nav-link" href="recetas.php">Recetas</a>
        <a class="nav-item nav-link" href="procesos.php">Procesos</a>
        <a class="nav-item nav-link active" href="consola.php">Consola</a>
        <button class="btn btn-danger" data-toggle="modal" data-target="#confirmacion">Cancelar Proceso</button>
      </nav>

        <div class="jumbotron jumbotron-fluid">
            <div class="container">
              <h1 class="display-4">Cervecero website</h1>
              <p class="lead">Esta es la interfaz de desarrollo del cervecero, desarrollada por Daniel Ubalde.</p>
            </div>
          </div>
        <h1>Seleccionar receta</h1>
        <div class="input-group">
          <input class="form-control" type="text" id="idPlacaReceta" placeholder="ID de placa">
          <select class="custom-select" id="receta">
            <option selected>Choose...</option>
          <?php
          $sql = $conn->query("SELECT ID, nombre FROM recetas");
          while ($valores = mysqli_fetch_array($sql)) {
             echo "<option value='$valores[ID]'>$valores[nombre]</option>";
          }
          ?>
        </select>
      <div class="input-group-append">
        <button class="btn btn-outline-primary" type="button" id="sendReceta">Cargar</button>
      </div>
    </div>
      <div class="collapse multi-collapse" id="recetacollapseOkay">
        <div class="alert alert-success">
          Enviado correctamente.
        </div>
      </div>
      <div class="collapse multi-collapse" id="recetacollapseFail">
        <div class="alert alert-danger">
          Error, no se pudo enviar
        </div>
      </div>
    
    <h1>Ejecutar proceso</h1>
    <div class="input-group">
      <input class="form-control" type="text" id="idPlacaProceso" placeholder="ID de placa">
      <select class="custom-select" id="proceso">
        <option selected>Choose...</option>
        <?php
        $sql = $conn->query("SELECT id, proceso FROM procesos");
        while ($valores = mysqli_fetch_array($sql)) {
            echo "<option value='$valores[id]'>$valores[proceso]</option>";
        }
        ?>
      </select>
      <div class="input-group-append">
    </div>
    <select class="custom-select" id="paso">
        <option selected>Choose...</option>
        <option value="0">1</option>
        <option value="1">2</option>
        <option value="2">3</option>
        <option value="3">4</option>
      </select>
      <div class="input-group-append">
        <button class="btn btn-outline-primary" type="button" id="sendProceso">Ejecutar</button>
      </div>
    </div>
    <div class="collapse multi-collapse" id="procesocollapseOkay">
      <div class="alert alert-success">
        Enviado correctamente.
      </div>
    </div>
        <div class="collapse multi-collapse" id="procesocollapseFail">
          <div class="alert alert-danger">
            Error, no se pudo enviar
          </div>
    </div>


    <h1>Ajustes</h1>
        <div class="input-group">
          <input class="form-control" type="text" id="idPlacaAjustes" placeholder="ID de placa">
        <select class="custom-select" id="settings">
          <option selected>Choose...</option>
          <option value="1">Poner en hora modulo RTC</option>
          <option value="2">Mostrar la hora del modulo RTC por serial</option>
      </select>
      <div class="input-group-append">
        <button class="btn btn-outline-primary" type="button" id="sendSettings">Cargar</button>
      </div>
    </div>
      <div class="collapse multi-collapse" id="settingcollapseOkay">
        <div class="alert alert-success">
          Enviado correctamente.
        </div>
      </div>
      <div class="collapse multi-collapse" id="settingcollapseFail">
        <div class="alert alert-danger">
          Error, no se pudo enviar
        </div>
      </div>

    <div class="modal fade" id="confirmacion" tabindex="-1" role="dialog">
        <div class="modal-dialog" role="document">
          <div class="modal-content">
            <div class="modal-header">
              <h5 class="modal-title">Cancelar proceso</h5>
              <button type="button" class="close" data-dismiss="modal" aria-label="Close">
                <span aria-hidden="true">&times;</span>
              </button>
            </div>
            <div class="modal-body">
              <p>¿Estas seguro de que quieres cancelar el proceso?</p>
              <input class="form-control" type="text" id="idPlacaCancelar" placeholder="ID de placa">
            </div>
            <div class="modal-footer">
              <button type="button" class="btn btn-secondary" data-dismiss="modal">Salir</button>
              <button type="button" class="btn btn-danger" data-dismiss="modal" id="cancelar">Cancelar</button>
            </div>
          </div>
        </div>
      </div>

    </div>
    
</body>
<!--Bootstrap-->
<script src="https://code.jquery.com/jquery-3.2.1.slim.min.js" integrity="sha384-KJ3o2DKtIkvYIK3UENzmM7KCkRr/rE9/Qpg6aAZGJwFDMVNA/GpGFF93hXpG5KkN" crossorigin="anonymous"></script>
<script src="https://cdnjs.cloudflare.com/ajax/libs/popper.js/1.12.9/umd/popper.min.js" integrity="sha384-ApNbgh9B+Y1QKtv3Rn7W3mgPxhU9K/ScQsAP7hUibX39j7fakFPskvXusvfa0b4Q" crossorigin="anonymous"></script>
<script src="https://maxcdn.bootstrapcdn.com/bootstrap/4.0.0/js/bootstrap.min.js" integrity="sha384-JZR6Spejh4U02d8jOt6vLEHfe/JQGiRRSQQxSfFWpi1MquVdAyjUar5+76PVCmYl" crossorigin="anonymous"></script>
<!--Custom-->
<script src="./js/main.js"></script>
</html>