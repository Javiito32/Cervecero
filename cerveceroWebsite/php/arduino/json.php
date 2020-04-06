<?php
require '../conexion.php';
$IDplaca = $_GET['IDplaca'];
if (!isset($_GET['reset'])) {
  $sql = $conn->query("SELECT menu,dato FROM menu WHERE IDplaca=$IDplaca");
while ($valores = mysqli_fetch_array($sql)){
    if ($valores['dato'] != 0 && $valores['menu'] != 0) {
        $arr = array("menu" => $valores['menu'], "dato" => $valores['dato']);
        echo json_encode($arr), "\n";
    }
}  
}else {
    $sql = $conn->query("UPDATE menu SET menu = 0, dato = 0 WHERE IDplaca = $IDplaca");
}
?>