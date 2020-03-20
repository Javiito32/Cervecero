<html>
<head>
<title>Cervecilla</title>
</head>
 
<body>
 <?php
 require 'conexion.php';
 $mac = "'84:0D:8E:B0:7C:53'"; 
 $sql = "UPDATE info SET fallo = 1 WHERE placaID=$mac";
if ($conn->query($sql)) {
    header('location:index.php');

} else {
    header('location:index.php?error=No se pudo cancelar');
}
?>
</div>
</body>
</html>