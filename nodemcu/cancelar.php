<html>
<head>
<title>Cervecilla</title>
</head>
 
<body>
 <?php
 require 'conexion.php';
 $sql = "UPDATE info SET fallo = 1 WHERE id=1";
if ($conn->query($sql)) {
    header('location:index.php');

} else {
    header('location:index.php?error=No se pudo cancelar');
}
?>
</div>
</body>
</html>