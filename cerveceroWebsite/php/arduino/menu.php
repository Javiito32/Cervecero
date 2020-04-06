<?php
require '../conexion.php';
$IDplaca = "'$_GET[IDplaca]'";
if (isset($_GET['resetmenu'])) {
    $sql = $conn->query("UPDATE menu SET menu = 0 WHERE IDplaca=$IDplaca");

}
if (isset($_GET['menu'])){
    $sql = $conn->query("SELECT menu FROM menu WHERE IDplaca=$IDplaca");
    while ($valores = mysqli_fetch_array($sql)) {
        echo "$valores[menu]";
    }
}

if (isset($_GET['resetfallo'])) {
    $sql = $conn->query("UPDATE menu SET fallo = 0 WHERE IDplaca=$IDplaca");
    
}
if (isset($_GET['fallo'])){
    $sql = $conn->query("SELECT fallo FROM menu WHERE IDplaca=$IDplaca");
    while ($valores = mysqli_fetch_array($sql)) {
        echo "$valores[fallo]";
    }
}
?>