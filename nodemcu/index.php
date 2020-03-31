<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1" />
<title>Cervecero</title>
</head>
<?php
if (isset($_GET['error'])) {
    echo"$_GET[error]";
}
?>
<body onload="menu.focus()">
<form action="receta.php" method="post">
<input type="text" name="menu" id="menu">
<input type="submit" value="Enviar">
</form>
<form action="cancelar.php" method="post" id="cancelar"></form>
<input type="button" value="Cancelar Proceso" onclick="document.getElementById('cancelar').submit()">
<form action="update.php" method="post" id="update"></form>
<input type="button" value="Actualizar firmware" onclick="document.getElementById('update').submit()">
</body>
</html>