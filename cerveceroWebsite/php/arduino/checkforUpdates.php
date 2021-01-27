<?php
require '../conexion.php';

$IDplaca = $_POST['IDplaca'];
$version = $_POST['currentVersion'];

if (isset($version)) {
    $sql = $conn->query("SELECT currentVersion FROM placas WHERE IDplaca = $IDplaca");
    while ($valores = mysqli_fetch_array($sql)) {
        if ($version != $valores['currentVersion']) {
            $sql = $conn->query("UPDATE placas SET currentVersion='$version' WHERE IDplaca=$IDplaca");
        }
    }
    
    $sql = $conn->query("SELECT version FROM firmwares ORDER BY fechaSalida DESC LIMIT 1");
    while ($valores = mysqli_fetch_array($sql)) {
        if ($version == $valores['version']) {
            $sql = $conn->query("UPDATE menu SET updateNextBoot=0, needUpdate=0 WHERE IDplaca=$IDplaca");
            $json = array("updateAvailable" => "0", "updateNow" => "0");
            //echo "0:0";
            //echo "El fimware está actualizado";
        }else{
            $sql = $conn->query("SELECT updateNextBoot FROM menu WHERE IDplaca=$IDplaca");
            while($valores = mysqli_fetch_array($sql)){
                if ($valores['updateNextBoot'] == 1) {
                    $json = array("updateAvailable" => "1", "updateNow" => "1");
                    //echo "1:1";
                    //echo "Va a actualizar";
                }else {
                    $sql = $conn->query("UPDATE menu SET needUpdate=1 WHERE IDplaca=$IDplaca");
                    $json = array("updateAvailable" => "1", "updateNow" => "0");
                    //echo "1:0";
                    //echo "Actualización disponible";
                }
            }

            
            
            //echo "Actualizacion disponible: Current version-$version ___ New version-$valores[version]";
        }

        echo json_encode($json);
    }
}
?>