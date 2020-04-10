<?php
require '../conexion.php';
$IDplaca = $_POST['IDplaca'];
if (!isset($_POST['reset'])) {
  $sql = $conn->query("SELECT menu, dato1, dato2 FROM menu WHERE IDplaca=$IDplaca");
while ($valores = mysqli_fetch_array($sql)){
        $arr = array("menu" => $valores['menu'], "dato1" => $valores['dato1'], "dato2" => $valores['dato2']);
        echo json_encode($arr), "\n";
}  
}else {
    $sql = $conn->query("UPDATE menu SET menu = 0, dato1 = 0, dato2 = 0 WHERE IDplaca = $IDplaca");
}
?>