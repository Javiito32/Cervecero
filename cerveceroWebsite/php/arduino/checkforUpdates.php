<?php
require '../conexion.php';
$IDplaca = $_POST['IDplaca'];
$version = $_POST['currentVersion'];
if (isset($_POST['currentVersion'])) {
    $sql = $conn->query("SELECT currentVersion FROM updates WHERE IDplaca = $IDplaca");
    while ($valores = mysqli_fetch_array($sql)) {
        if ($version != $valores['currentVersion']) {
            $sql = $conn->query("UPDATE updates SET currentVersion='$version' WHERE IDplaca=$IDplaca");
        }
    }
    
    $sql = $conn->query("SELECT version FROM firmwares ORDER BY fechaSalida DESC LIMIT 1");
    while ($valores = mysqli_fetch_array($sql)) {
        if ($version == $valores['version']) {
            $sql = $conn->query("UPDATE placas SET needUpdate=0 WHERE IDplaca=$IDplaca");
            $sql = $conn->query("UPDATE menu SET updateNextBoot=0 WHERE IDplaca=$IDplaca");
            echo "0:0";
            //echo "El fimware está actualizado";
        }else{
            $sql = $conn->query("SELECT updateNextBoot FROM menu WHERE IDplaca=$IDplaca");
            while($valores = mysqli_fetch_array($sql)){
                if ($valores['updateNextBoot'] == 1) {
                    $updateNextBoot = 1;
                    echo "1:1";
                }else {
                    $sql = $conn->query("UPDATE placas SET needUpdate=1 WHERE IDplaca=$IDplaca");
                    echo "1:0";
                }
            }

            
            
            //echo "Actualizacion disponible: Current version-$version ___ New version-$valores[version]";
        }
    }
}
?>