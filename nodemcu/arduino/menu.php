<?php
require '../conexion.php';
$mac = "'$_GET[placaID]'";
if (isset($_GET['resetmenu'])) {
    $sql = $conn->query("UPDATE info SET menu = 0 WHERE placaID=$mac");

}
if (isset($_GET['menu'])){
    $sql = $conn->query("SELECT menu FROM info WHERE placaID=$mac");
    while ($valores = mysqli_fetch_array($sql)) {
        echo "$valores[menu]";
    }
}

if (isset($_GET['resetfallo'])) {
    $sql = $conn->query("UPDATE info SET fallo = 0 WHERE placaID=$mac");
    
}
if (isset($_GET['fallo'])){
    $sql = $conn->query("SELECT fallo FROM info WHERE placaID=$mac");
    while ($valores = mysqli_fetch_array($sql)) {
        echo "$valores[fallo]";
    }
}
?>