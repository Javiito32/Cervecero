<html>
<head>
<title>Cervecilla</title>
</head>
 
<body>
 <?php
 require 'conexion.php'; 
 $sql = "UPDATE menu SET updateNextBoot = 1 WHERE IDplaca=1";
if ($conn->query($sql)) {
    header('location:index.php');

} else {
    header('location:index.php?error=No se pudo actualizar');
}
?>
</div>
</body>
</html>