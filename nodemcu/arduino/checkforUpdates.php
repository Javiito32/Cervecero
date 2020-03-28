<?php
require '../conexion.php';
$IDplaca = $_GET['IDplaca'];
$version = $_GET['currentVersion'];
if (isset($_GET['currentVersion'])) {
    $sql = $conn->query("UPDATE updates SET currentVersion='$version' WHERE IDplaca=$IDplaca");
    $sql = $conn->query("SELECT version FROM firmwares ORDER BY fechaSalida DESC LIMIT 1");
    while ($valores = mysqli_fetch_array($sql)) {
        if ($version == $valores['version']) {
            $sql = $conn->query("UPDATE placas SET needUpdate=0 WHERE IDplaca=$IDplaca");
            $sql = $conn->query("UPDATE menu SET updateNextBoot=0 WHERE IDplaca=$IDplaca");
            echo "0";
            //echo "El fimware está actualizado";
        }else{
            $sql = $conn->query("UPDATE placas SET needUpdate=1 WHERE IDplaca=$IDplaca");
            echo "1";
            //echo "Actualizacion disponible: Current version-$version ___ New version-$valores[version]";
        }
    }
}
?>