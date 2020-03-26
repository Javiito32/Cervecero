<html>
<head>
<title>Cervecilla</title>
</head>
 
<body>
 <?php
 require 'conexion.php';
 $menu = htmlentities(addslashes($_POST["menu"]));
 $sql = "UPDATE menu SET menu = $menu WHERE IDplaca=1";
if ($conn->query($sql)) {
    header('location:index.php');

} else {
    header('location:index.php?error=No se encontró la receta');
}
?>
</div>
</body>
</html>