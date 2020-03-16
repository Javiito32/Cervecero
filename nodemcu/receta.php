<html>
<head>
<title>Cervecilla</title>
</head>
 
<body>
 <?php
 require 'conexion.php';
 $receta = htmlentities(addslashes($_POST["receta"]));
 $sql = "UPDATE info SET receta = $receta WHERE id=1";
if ($conn->query($sql)) {
    header('location:index.php');

} else {
    header('location:index.php?error=No se encontró la receta');
}
?>
</div>
</body>
</html>