<html>
<head>
<title>Cervecilla</title>
</head>
 
<body>
 <?php
 require 'conexion.php';
 $mac = "'84:0D:8E:B0:7C:53'";
 $menu = htmlentities(addslashes($_POST["menu"]));
 $sql = "UPDATE info SET menu = $menu WHERE placaID=$mac";
if ($conn->query($sql)) {
    header('location:index.php');

} else {
    header('location:index.php?error=No se encontró la receta');
}
?>
</div>
</body>
</html>