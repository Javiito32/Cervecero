<?php
require '../conexion.php';
$IDplaca = "'$_POST[IDplaca]'";
if (isset($_POST['resetmenu'])) {
    $sql = $conn->query("UPDATE menu SET menu = 0 WHERE IDplaca=$IDplaca");

}
if (isset($_POST['menu'])){
    $sql = $conn->query("SELECT menu FROM menu WHERE IDplaca=$IDplaca");
    while ($valores = mysqli_fetch_array($sql)) {
        echo "$valores[menu]";
    }
}

if (isset($_POST['resetfallo'])) {
    $sql = $conn->query("UPDATE menu SET fallo = 0 WHERE IDplaca=$IDplaca");
    
}
if (isset($_POST['fallo'])){
    $sql = $conn->query("SELECT fallo FROM menu WHERE IDplaca=$IDplaca");
    while ($valores = mysqli_fetch_array($sql)) {
        echo "$valores[fallo]";
    }
}
?>