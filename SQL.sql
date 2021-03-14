-- --------------------------------------------------------
-- Host:                         192.168.1.230
-- Versión del servidor:         10.1.47-MariaDB-0ubuntu0.18.04.1 - Ubuntu 18.04
-- SO del servidor:              debian-linux-gnueabihf
-- HeidiSQL Versión:             11.2.0.6213
-- --------------------------------------------------------

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!50503 SET NAMES utf8mb4 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;


-- Volcando estructura de base de datos para cervecero
CREATE DATABASE IF NOT EXISTS `cervecero` /*!40100 DEFAULT CHARACTER SET utf8 COLLATE utf8_unicode_ci */;
USE `cervecero`;

-- Volcando estructura para tabla cervecero.estados
CREATE TABLE IF NOT EXISTS `estados` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `estado` varchar(50) CHARACTER SET utf8mb4 DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=5 DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

-- Volcando datos para la tabla cervecero.estados: ~4 rows (aproximadamente)
/*!40000 ALTER TABLE `estados` DISABLE KEYS */;
REPLACE INTO `estados` (`id`, `estado`) VALUES
	(1, 'Iniciado'),
	(2, 'Finalizado'),
	(3, 'Cancelado'),
	(4, 'No iniciado');
/*!40000 ALTER TABLE `estados` ENABLE KEYS */;

-- Volcando estructura para tabla cervecero.intervenciones
CREATE TABLE IF NOT EXISTS `intervenciones` (
  `id_Paso_Receta` int(11) NOT NULL,
  `id_Receta` int(11) NOT NULL,
  `on_Minute` int(11) DEFAULT NULL,
  `instruccion` varchar(500) COLLATE utf8_unicode_ci DEFAULT NULL,
  `done` tinyint(1) DEFAULT NULL,
  PRIMARY KEY (`id_Paso_Receta`,`id_Receta`) USING BTREE,
  KEY `FK_intervenciones_recetas` (`id_Receta`),
  CONSTRAINT `FK_intervenciones_recetas` FOREIGN KEY (`id_Receta`) REFERENCES `recetas` (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

-- Volcando datos para la tabla cervecero.intervenciones: ~1 rows (aproximadamente)
/*!40000 ALTER TABLE `intervenciones` DISABLE KEYS */;
REPLACE INTO `intervenciones` (`id_Paso_Receta`, `id_Receta`, `on_Minute`, `instruccion`, `done`) VALUES
	(3, 3, 2880, 'Añadir la fruta', NULL);
/*!40000 ALTER TABLE `intervenciones` ENABLE KEYS */;

-- Volcando estructura para tabla cervecero.log_Placas_Firmwares
CREATE TABLE IF NOT EXISTS `log_Placas_Firmwares` (
  `id_Placa` int(11) NOT NULL,
  `fecha_update` timestamp NULL DEFAULT CURRENT_TIMESTAMP,
  `oldVersion` varchar(10) COLLATE utf8_unicode_ci DEFAULT NULL,
  `newVersion` varchar(10) COLLATE utf8_unicode_ci DEFAULT NULL,
  `id` int(11) NOT NULL AUTO_INCREMENT,
  PRIMARY KEY (`id`),
  KEY `FK_log_updates_placas` (`id_Placa`) USING BTREE,
  CONSTRAINT `FK_log_Placas_Firmwares_placas` FOREIGN KEY (`id_Placa`) REFERENCES `placas` (`id_Placa`)
) ENGINE=InnoDB AUTO_INCREMENT=14 DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

-- Volcando datos para la tabla cervecero.log_Placas_Firmwares: ~1 rows (aproximadamente)
/*!40000 ALTER TABLE `log_Placas_Firmwares` DISABLE KEYS */;
REPLACE INTO `log_Placas_Firmwares` (`id_Placa`, `fecha_update`, `oldVersion`, `newVersion`, `id`) VALUES
	(2, '2021-02-27 13:00:48', '0.0.5', '0.0.6', 13);
/*!40000 ALTER TABLE `log_Placas_Firmwares` ENABLE KEYS */;

-- Volcando estructura para tabla cervecero.menu
CREATE TABLE IF NOT EXISTS `menu` (
  `IDplaca` int(11) NOT NULL DEFAULT '0',
  `menu` int(11) NOT NULL DEFAULT '0',
  `dato1` int(11) NOT NULL DEFAULT '0',
  `dato2` int(11) NOT NULL DEFAULT '0',
  `needUpdate` int(1) NOT NULL DEFAULT '0',
  `updateNextBoot` int(11) NOT NULL DEFAULT '0',
  `continuar` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`IDplaca`) USING BTREE,
  CONSTRAINT `FK_menu_placas` FOREIGN KEY (`IDplaca`) REFERENCES `placas` (`id_Placa`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

-- Volcando datos para la tabla cervecero.menu: ~0 rows (aproximadamente)
/*!40000 ALTER TABLE `menu` DISABLE KEYS */;
REPLACE INTO `menu` (`IDplaca`, `menu`, `dato1`, `dato2`, `needUpdate`, `updateNextBoot`, `continuar`) VALUES
	(2, 2, 1, 0, 0, 0, 0);
/*!40000 ALTER TABLE `menu` ENABLE KEYS */;

-- Volcando estructura para tabla cervecero.pasos_Recetas
CREATE TABLE IF NOT EXISTS `pasos_Recetas` (
  `id_Paso_Receta` int(11) NOT NULL,
  `id_Receta` int(11) NOT NULL,
  `id_Proceso` int(11) DEFAULT NULL,
  `id_Paso_Proceso` int(11) DEFAULT NULL,
  `tiempo` varchar(50) COLLATE utf8_unicode_ci DEFAULT NULL,
  `instrucciones` varchar(200) COLLATE utf8_unicode_ci DEFAULT NULL,
  `intervencion` tinyint(4) DEFAULT '0',
  PRIMARY KEY (`id_Receta`,`id_Paso_Receta`) USING BTREE,
  KEY `FK_pasos_Recetas_procesos` (`id_Proceso`),
  CONSTRAINT `FK_pasos_Recetas_procesos` FOREIGN KEY (`id_Proceso`) REFERENCES `procesos` (`id`),
  CONSTRAINT `FK_pasos_Recetas_recetas` FOREIGN KEY (`id_Receta`) REFERENCES `recetas` (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

-- Volcando datos para la tabla cervecero.pasos_Recetas: ~11 rows (aproximadamente)
/*!40000 ALTER TABLE `pasos_Recetas` DISABLE KEYS */;
REPLACE INTO `pasos_Recetas` (`id_Paso_Receta`, `id_Receta`, `id_Proceso`, `id_Paso_Proceso`, `tiempo`, `instrucciones`, `intervencion`) VALUES
	(1, 1, 1, 0, '1 Minuto', 'Primera maceracion', 0),
	(2, 1, 2, 0, '5 Minutos', 'Primera Coccion', 0),
	(1, 3, 1, 0, '60 Minutos', 'Macerar las maltas durante 60 minutos a 65 ºC. Usar 12,5 litros de agua.', 0),
	(2, 3, 2, 0, '70 Minutos', 'Lavar y recircular para obtener el mosto. Añadir agua hasta alcanzar los 27 litros. Hervir durante 70 minutos, añadiendo el lúpulo al inicio del hervido.', 0),
	(3, 3, 3, 0, 'Boton', 'Pulsar el boton cuando el líquido se haya vaciado por completo.', 0),
	(4, 3, 4, 0, '1 Semana', 'Añadir la levadura Kveik y fermentar entre 30 y 40 ºC. Añadir la fruta a los 4 días de fermentación, retirarla tras una semana y dejar que termine de fermentar.', 1),
	(5, 3, 5, 0, '2 Semanas', 'Madurar durante 2 semanas a 12 ºC.', 0),
	(1, 4, 1, 0, '60 Minutos', 'Macerar las maltas durante 60 minutos a 65 ºC. Usar 16,25 litros de agua.', 0),
	(2, 4, 2, 0, '70 Minutos', 'Lavar y recircular hasta obtener el mosto. Añadir agua hasta alcanzar los 27 litros. Hervir durante 70 minutos, añadiendo los lúpulos tal y como se explica anteriormente.', 0),
	(3, 4, 3, 0, 'Boton', 'Pulsar el boton cuando el líquido se haya vaciado por completo.', 0),
	(4, 4, 4, 0, 'Esperar Gravedad', 'Hasta que la gravedad se cumpla', 0),
	(5, 4, 5, 0, '6 Semanas', 'Madurar durante 6 semanas a 12 ºC.', 0);
/*!40000 ALTER TABLE `pasos_Recetas` ENABLE KEYS */;

-- Volcando estructura para tabla cervecero.pasos_Recetas_Users
CREATE TABLE IF NOT EXISTS `pasos_Recetas_Users` (
  `id_Paso_Receta` int(11) NOT NULL DEFAULT '0',
  `id_Receta` int(11) DEFAULT NULL,
  `id_User` int(10) unsigned NOT NULL,
  `id_Estado` int(11) DEFAULT '4',
  PRIMARY KEY (`id_Paso_Receta`) USING BTREE,
  KEY `FK_pasos_Receta_Users_users` (`id_User`),
  KEY `FK_pasos_Recetas_Users_recetas` (`id_Receta`) USING BTREE,
  KEY `FK_pasos_Receta_Users_estados` (`id_Estado`) USING BTREE,
  CONSTRAINT `FK_pasos_Receta_Users_users` FOREIGN KEY (`id_User`) REFERENCES `users` (`id_User`),
  CONSTRAINT `FK_pasos_Recetas_Users_estados` FOREIGN KEY (`id_Estado`) REFERENCES `estados` (`id`),
  CONSTRAINT `FK_pasos_Recetas_Users_recetas` FOREIGN KEY (`id_receta`) REFERENCES `recetas` (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

-- Volcando datos para la tabla cervecero.pasos_Recetas_Users: ~0 rows (aproximadamente)
/*!40000 ALTER TABLE `pasos_Recetas_Users` DISABLE KEYS */;
/*!40000 ALTER TABLE `pasos_Recetas_Users` ENABLE KEYS */;

-- Volcando estructura para tabla cervecero.placas
CREATE TABLE IF NOT EXISTS `placas` (
  `id_Placa` int(11) NOT NULL AUTO_INCREMENT,
  `mac` varchar(17) CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci NOT NULL,
  `id_User` int(11) unsigned DEFAULT NULL,
  `updateNextBoot` tinyint(1) NOT NULL DEFAULT '0',
  `firmware` varchar(10) COLLATE utf8_unicode_ci DEFAULT NULL,
  PRIMARY KEY (`id_Placa`) USING BTREE,
  KEY `FK_placas_users` (`id_User`),
  CONSTRAINT `FK_placas_users` FOREIGN KEY (`id_User`) REFERENCES `users` (`id_User`)
) ENGINE=InnoDB AUTO_INCREMENT=3 DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

-- Volcando datos para la tabla cervecero.placas: ~1 rows (aproximadamente)
/*!40000 ALTER TABLE `placas` DISABLE KEYS */;
REPLACE INTO `placas` (`id_Placa`, `mac`, `id_User`, `updateNextBoot`, `firmware`) VALUES
	(2, 'C4:4F:33:76:F1:45', 1, 0, '0.0.6');
/*!40000 ALTER TABLE `placas` ENABLE KEYS */;

-- Volcando estructura para tabla cervecero.placas_data
CREATE TABLE IF NOT EXISTS `placas_data` (
  `ID` int(11) NOT NULL AUTO_INCREMENT,
  `id_Placa` int(11) DEFAULT NULL,
  `receta` int(11) DEFAULT NULL,
  `time` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `proceso` int(11) DEFAULT NULL,
  `pasoProceso` int(11) DEFAULT NULL,
  `estado` int(11) DEFAULT NULL,
  `tiempoRestante` int(11) DEFAULT NULL,
  `porcentaje` tinyint(4) DEFAULT NULL,
  `temp` float DEFAULT NULL,
  PRIMARY KEY (`ID`),
  KEY `estado` (`estado`),
  KEY `proceso` (`proceso`),
  KEY `FK_log_recetas` (`receta`),
  KEY `placaID` (`id_Placa`) USING BTREE,
  CONSTRAINT `FK_log_estados` FOREIGN KEY (`estado`) REFERENCES `estados` (`id`) ON DELETE SET NULL ON UPDATE CASCADE,
  CONSTRAINT `FK_log_placas` FOREIGN KEY (`id_Placa`) REFERENCES `placas` (`id_Placa`) ON DELETE SET NULL ON UPDATE CASCADE,
  CONSTRAINT `FK_log_procesos` FOREIGN KEY (`proceso`) REFERENCES `procesos` (`id`) ON DELETE SET NULL ON UPDATE CASCADE,
  CONSTRAINT `FK_log_recetas` FOREIGN KEY (`receta`) REFERENCES `recetas` (`ID`) ON DELETE SET NULL ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

-- Volcando datos para la tabla cervecero.placas_data: ~0 rows (aproximadamente)
/*!40000 ALTER TABLE `placas_data` DISABLE KEYS */;
/*!40000 ALTER TABLE `placas_data` ENABLE KEYS */;

-- Volcando estructura para tabla cervecero.placas_firmwares
CREATE TABLE IF NOT EXISTS `placas_firmwares` (
  `version` varchar(10) CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci NOT NULL DEFAULT '0.0.0',
  `fechaSalida` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  PRIMARY KEY (`version`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

-- Volcando datos para la tabla cervecero.placas_firmwares: ~1 rows (aproximadamente)
/*!40000 ALTER TABLE `placas_firmwares` DISABLE KEYS */;
REPLACE INTO `placas_firmwares` (`version`, `fechaSalida`) VALUES
	('0.0.6', '2020-12-08 21:18:58');
/*!40000 ALTER TABLE `placas_firmwares` ENABLE KEYS */;

-- Volcando estructura para tabla cervecero.procesos
CREATE TABLE IF NOT EXISTS `procesos` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `proceso` varchar(17) CHARACTER SET utf8mb4 DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=6 DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

-- Volcando datos para la tabla cervecero.procesos: ~4 rows (aproximadamente)
/*!40000 ALTER TABLE `procesos` DISABLE KEYS */;
REPLACE INTO `procesos` (`id`, `proceso`) VALUES
	(1, 'Maceracion'),
	(2, 'Coccion'),
	(3, 'Transvase'),
	(4, 'Fermentacion'),
	(5, 'Reposo');
/*!40000 ALTER TABLE `procesos` ENABLE KEYS */;

-- Volcando estructura para tabla cervecero.recetas
CREATE TABLE IF NOT EXISTS `recetas` (
  `ID` int(11) NOT NULL AUTO_INCREMENT,
  `nombre` varchar(50) CHARACTER SET utf8mb4 NOT NULL DEFAULT '0',
  `tempMacer` tinyint(4) NOT NULL DEFAULT '0',
  `tiempoMacer` int(11) NOT NULL DEFAULT '0',
  `tempCoc` tinyint(4) NOT NULL DEFAULT '0',
  `tiempoCoc` int(11) NOT NULL DEFAULT '0',
  `tempFermen` tinyint(4) NOT NULL DEFAULT '0',
  `tiempoFermen` int(11) NOT NULL DEFAULT '0',
  `tempReposo` int(11) DEFAULT '0',
  `tiempoReposo` int(11) DEFAULT '0',
  `gravedad_Inicial` int(11) DEFAULT '0',
  `gravedad_Final` int(11) DEFAULT '0',
  `src` varchar(50) COLLATE utf8_unicode_ci DEFAULT NULL,
  `descripcion` varchar(500) COLLATE utf8_unicode_ci DEFAULT NULL,
  `visible` tinyint(1) DEFAULT '0',
  `ingredients` varchar(200) COLLATE utf8_unicode_ci DEFAULT NULL,
  `fuente` varchar(200) COLLATE utf8_unicode_ci DEFAULT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=5 DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

-- Volcando datos para la tabla cervecero.recetas: ~4 rows (aproximadamente)
/*!40000 ALTER TABLE `recetas` DISABLE KEYS */;
REPLACE INTO `recetas` (`ID`, `nombre`, `tempMacer`, `tiempoMacer`, `tempCoc`, `tiempoCoc`, `tempFermen`, `tiempoFermen`, `tempReposo`, `tiempoReposo`, `gravedad_Inicial`, `gravedad_Final`, `src`, `descripcion`, `visible`, `ingredients`, `fuente`) VALUES
	(1, 'Pinta', 54, 1, 30, 5, 42, 3, NULL, NULL, NULL, NULL, 'img/beers/pinta.jpg', 'Es decir, algo más de medio litro. En Estados Unidos, en cambio, la medida es algo inferior, 473 ml. Por tanto, no llega al medio litro en el país americano.', 0, NULL, NULL),
	(2, 'Alcahchofa', 78, 2, 74, 10, 15, 12, NULL, NULL, 1051, 1010, NULL, 'Receta de pruebas', 0, NULL, NULL),
	(3, 'Afrutada', 65, 60, 100, 70, 35, 10080, 12, 20160, 1045, 1010, 'img/beers/afrutada.jpg', 'Las cervezas con fruta se suelen preparar con malta de trigo y frutas que le dan a la cerveza una turbidez sustancial. Proceso de elaboración: 4 semanas Alcohol: 5,7% Amargor: 15 IBU', 1, '{"ingredients": ["Hola","Alcachofa","asdasdasdasdasdasda"]}', 'https://hacercervezaartesanal.com/como-preparar-cerveza-con-frutas/'),
	(4, 'Belgain Blonde Ale', 65, 60, 100, 70, 22, 0, 12, 60480, 1070, 1015, 'img/beers/belgian-blonde-ale.jpg', 'La Belgian Blonde Ale es una cerveza fuerte de color amarillo claro pajizo. Una cerveza originaria de Bélgica típicamente elaborada en abadías. Tiene un dulzor característico que le dan las maltas y los cristales de azúcar Candy, pero que se balancea con un aroma especiado y algo lupulado. Para conseguir ese sabor y aroma característico de abadía se tiene que usar levaduras belgas de abadía.', 1, NULL, 'https://hacercervezaartesanal.com/receta-belgian-blonde-ale/');
/*!40000 ALTER TABLE `recetas` ENABLE KEYS */;

-- Volcando estructura para tabla cervecero.sondas
CREATE TABLE IF NOT EXISTS `sondas` (
  `id_Sonda` int(11) NOT NULL AUTO_INCREMENT,
  `id_User` int(11) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`id_Sonda`),
  KEY `FK_sodas_users` (`id_User`),
  CONSTRAINT `FK_sodas_users` FOREIGN KEY (`id_User`) REFERENCES `users` (`id_User`)
) ENGINE=InnoDB AUTO_INCREMENT=3 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;

-- Volcando datos para la tabla cervecero.sondas: ~0 rows (aproximadamente)
/*!40000 ALTER TABLE `sondas` DISABLE KEYS */;
REPLACE INTO `sondas` (`id_Sonda`, `id_User`) VALUES
	(2, 1);
/*!40000 ALTER TABLE `sondas` ENABLE KEYS */;

-- Volcando estructura para tabla cervecero.sondas_data
CREATE TABLE IF NOT EXISTS `sondas_data` (
  `id` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `id_Sonda` int(11) NOT NULL DEFAULT '0',
  `tilt` float NOT NULL DEFAULT '0',
  `temp` float NOT NULL DEFAULT '0',
  `temp_Unit` varchar(50) COLLATE utf8_unicode_ci NOT NULL,
  `battery` float NOT NULL DEFAULT '0',
  `gravity` float NOT NULL DEFAULT '0',
  `interval` tinyint(4) NOT NULL DEFAULT '0',
  `RSSI` float NOT NULL DEFAULT '0',
  `time` timestamp NULL DEFAULT CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`),
  KEY `id_Sonda` (`id_Sonda`),
  CONSTRAINT `FK_sonda_data_sodas` FOREIGN KEY (`id_Sonda`) REFERENCES `sondas` (`id_Sonda`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

-- Volcando datos para la tabla cervecero.sondas_data: ~0 rows (aproximadamente)
/*!40000 ALTER TABLE `sondas_data` DISABLE KEYS */;
/*!40000 ALTER TABLE `sondas_data` ENABLE KEYS */;

-- Volcando estructura para tabla cervecero.users
CREATE TABLE IF NOT EXISTS `users` (
  `id_User` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `user_Name` varchar(255) CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci DEFAULT NULL,
  `phone` varchar(100) CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci DEFAULT NULL,
  `pass` varchar(60) COLLATE utf8_unicode_ci NOT NULL,
  `email` varchar(60) COLLATE utf8_unicode_ci NOT NULL,
  PRIMARY KEY (`id_User`)
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

-- Volcando datos para la tabla cervecero.users: ~0 rows (aproximadamente)
/*!40000 ALTER TABLE `users` DISABLE KEYS */;
REPLACE INTO `users` (`id_User`, `user_Name`, `phone`, `pass`, `email`) VALUES
	(1, 'Kerjox', '111111111', '$2y$10$cU.9SseE4nD18djgbCLx8OqiVMBztjTh37VdRNBpOFxQXk3yrFKMm', 'danielubaldelaguia@gmail.com');
/*!40000 ALTER TABLE `users` ENABLE KEYS */;

-- Volcando estructura para disparador cervecero.placas_AU
SET @OLDTMP_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION';
DELIMITER //
CREATE TRIGGER `placas_AU` AFTER UPDATE ON `placas` FOR EACH ROW BEGIN
INSERT INTO log_Placas_Firmwares (id_Placa, oldVersion, newVersion) VALUES (new.id_Placa, OLD.firmware, NEW.firmware);
END//
DELIMITER ;
SET SQL_MODE=@OLDTMP_SQL_MODE;

/*!40101 SET SQL_MODE=IFNULL(@OLD_SQL_MODE, '') */;
/*!40014 SET FOREIGN_KEY_CHECKS=IFNULL(@OLD_FOREIGN_KEY_CHECKS, 1) */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40111 SET SQL_NOTES=IFNULL(@OLD_SQL_NOTES, 1) */;
