<?php
require '../conexion.php';
$IDplaca = $_GET['IDplaca'];
$sql = $conn->query("SELECT updateNextBoot FROM menu WHERE IDplaca=$IDplaca");
while ($valores = mysqli_fetch_array($sql)) {
    if ($valores['updateNextBoot'] == 1) {
        echo "1";
    }else {
        echo "0";
    }
}
?>