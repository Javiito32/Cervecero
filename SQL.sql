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
	(4, 'No inciado');
/*!40000 ALTER TABLE `estados` ENABLE KEYS */;

-- Volcando estructura para tabla cervecero.intervenciones
CREATE TABLE IF NOT EXISTS `intervenciones` (
  `id_Paso_Receta` int(11) NOT NULL,
  `id_Receta` int(11) NOT NULL,
  `on_Time` int(11) DEFAULT NULL,
  PRIMARY KEY (`id_Paso_Receta`,`id_Receta`) USING BTREE,
  KEY `FK_intervenciones_recetas` (`id_Receta`),
  CONSTRAINT `FK_intervenciones_recetas` FOREIGN KEY (`id_Receta`) REFERENCES `recetas` (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

-- Volcando datos para la tabla cervecero.intervenciones: ~0 rows (aproximadamente)
/*!40000 ALTER TABLE `intervenciones` DISABLE KEYS */;
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
) ENGINE=InnoDB AUTO_INCREMENT=11 DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

-- Volcando datos para la tabla cervecero.log_Placas_Firmwares: ~1 rows (aproximadamente)
/*!40000 ALTER TABLE `log_Placas_Firmwares` DISABLE KEYS */;
REPLACE INTO `log_Placas_Firmwares` (`id_Placa`, `fecha_update`, `oldVersion`, `newVersion`, `id`) VALUES
	(2, '2021-02-15 00:11:57', '0.0.5', '0.0.6', 10);
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

-- Volcando datos para la tabla cervecero.pasos_Recetas: ~6 rows (aproximadamente)
/*!40000 ALTER TABLE `pasos_Recetas` DISABLE KEYS */;
REPLACE INTO `pasos_Recetas` (`id_Paso_Receta`, `id_Receta`, `id_Proceso`, `id_Paso_Proceso`, `tiempo`, `instrucciones`, `intervencion`) VALUES
	(1, 1, 1, 0, '1 Minuto', 'Primera maceracion', 0),
	(2, 1, 2, 0, '5 Minutos', 'Primera Coccion', 0),
	(1, 3, 1, 0, '60 Minutos', 'Macerar las maltas durante 60 minutos a 65 ºC. Usar 12,5 litros de agua.', 0),
	(2, 3, 2, 0, '70 Minutos', 'Lavar y recircular para obtener el mosto. Añadir agua hasta alcanzar los 27 litros. Hervir durante 70 minutos, añadiendo el lúpulo al inicio del hervido.', 0),
	(3, 3, 4, 0, '1 Semana', 'Añadir la levadura Kveik y fermentar entre 30 y 40 ºC. Añadir la fruta a los 4 días de fermentación, retirarla tras una semana y dejar que termine de fermentar.', 0),
	(4, 3, 5, 0, '2 Semanas', 'Madurar durante 2 semanas a 12 ºC.', 0);
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

-- Volcando datos para la tabla cervecero.pasos_Recetas_Users: ~4 rows (aproximadamente)
/*!40000 ALTER TABLE `pasos_Recetas_Users` DISABLE KEYS */;
REPLACE INTO `pasos_Recetas_Users` (`id_Paso_Receta`, `id_Receta`, `id_User`, `id_Estado`) VALUES
	(1, 3, 1, 4),
	(2, 3, 1, 4),
	(3, 3, 1, 4),
	(4, 3, 1, 4);
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
) ENGINE=InnoDB AUTO_INCREMENT=963 DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

-- Volcando datos para la tabla cervecero.placas_data: ~0 rows (aproximadamente)
/*!40000 ALTER TABLE `placas_data` DISABLE KEYS */;
/*!40000 ALTER TABLE `placas_data` ENABLE KEYS */;

-- Volcando estructura para tabla cervecero.placas_firmwares
CREATE TABLE IF NOT EXISTS `placas_firmwares` (
  `version` varchar(10) CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci NOT NULL DEFAULT '0.0.0',
  `fechaSalida` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  PRIMARY KEY (`version`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

-- Volcando datos para la tabla cervecero.placas_firmwares: ~0 rows (aproximadamente)
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

-- Volcando datos para la tabla cervecero.procesos: ~5 rows (aproximadamente)
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
  `tempMacer` varchar(50) CHARACTER SET utf8mb4 NOT NULL DEFAULT '0',
  `tiempoMacer` varchar(50) CHARACTER SET utf8mb4 NOT NULL DEFAULT '0',
  `tempCoc` varchar(50) CHARACTER SET utf8mb4 NOT NULL DEFAULT '0',
  `tiempoCoc` varchar(50) CHARACTER SET utf8mb4 NOT NULL DEFAULT '0',
  `tempFermen` varchar(50) CHARACTER SET utf8mb4 NOT NULL DEFAULT '0',
  `tiempoFermen` varchar(50) CHARACTER SET utf8mb4 NOT NULL DEFAULT '0',
  `tempReposo` int(11) DEFAULT NULL,
  `tiempoReposo` int(11) DEFAULT NULL,
  `gravedad_Inicial` int(11) DEFAULT NULL,
  `src` varchar(50) COLLATE utf8_unicode_ci DEFAULT NULL,
  `descripcion` varchar(500) COLLATE utf8_unicode_ci DEFAULT NULL,
  `gravedad_Final` int(11) DEFAULT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=4 DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

-- Volcando datos para la tabla cervecero.recetas: ~3 rows (aproximadamente)
/*!40000 ALTER TABLE `recetas` DISABLE KEYS */;
REPLACE INTO `recetas` (`ID`, `nombre`, `tempMacer`, `tiempoMacer`, `tempCoc`, `tiempoCoc`, `tempFermen`, `tiempoFermen`, `tempReposo`, `tiempoReposo`, `gravedad_Inicial`, `src`, `descripcion`, `gravedad_Final`) VALUES
	(1, 'Pinta', '54:78:89:34', '1:5:7:3', '30', '1', '42', '3', NULL, NULL, NULL, 'img/beers/pinta.jpg', 'Es decir, algo más de medio litro. En Estados Unidos, en cambio, la medida es algo inferior, 473 ml. Por tanto, no llega al medio litro en el país americano.', NULL),
	(2, 'Alcahchofa', '78:45:56:12', '2:5:7:10', '74:41:10:45', '10:45:41:23', '15:45:78:98', '12:45:78:89', NULL, NULL, NULL, NULL, 'Receta de pruebas', NULL),
	(3, 'Afrutada', '65', '60', '100', '70', '35', '10080', 12, 20160, 1045, 'img/beers/afrutada.jpg', 'Las cervezas con fruta se suelen preparar con malta de trigo y frutas que le dan a la cerveza una turbidez sustancial. Proceso de elaboración: 4 semanas Alcohol: 5,7% Amargor: 15 IBU', 1010);
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
  `temp` int(11) NOT NULL DEFAULT '0',
  `temp_Unit` varchar(50) COLLATE utf8_unicode_ci NOT NULL,
  `battery` float NOT NULL DEFAULT '0',
  `gravity` float NOT NULL DEFAULT '0',
  `interval` tinyint(4) NOT NULL DEFAULT '0',
  `RSSI` float NOT NULL DEFAULT '0',
  `date` timestamp NULL DEFAULT CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`),
  KEY `id_Sonda` (`id_Sonda`),
  CONSTRAINT `FK_sonda_data_sodas` FOREIGN KEY (`id_Sonda`) REFERENCES `sondas` (`id_Sonda`)
) ENGINE=InnoDB AUTO_INCREMENT=660 DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

-- Volcando datos para la tabla cervecero.sondas_data: ~182 rows (aproximadamente)
/*!40000 ALTER TABLE `sondas_data` DISABLE KEYS */;
REPLACE INTO `sondas_data` (`id`, `id_Sonda`, `tilt`, `temp`, `temp_Unit`, `battery`, `gravity`, `interval`, `RSSI`, `date`) VALUES
	(472, 2, 177.81, 22, 'C', 3.91032, 75.185, 5, -28, '2021-02-10 17:46:21'),
	(473, 2, 177.8, 22, 'C', 3.91032, 75.1805, 5, -28, '2021-02-10 17:46:27'),
	(474, 2, 177.804, 22, 'C', 3.91032, 75.1823, 5, -29, '2021-02-10 17:46:34'),
	(475, 2, 177.818, 22, 'C', 3.91554, 75.1885, 5, -29, '2021-02-10 17:46:44'),
	(476, 2, 177.813, 22, 'C', 3.91032, 75.1866, 5, -28, '2021-02-10 17:46:50'),
	(477, 2, 177.816, 22, 'C', 3.91554, 75.1876, 5, -29, '2021-02-10 17:46:56'),
	(478, 2, 177.805, 22, 'C', 3.91032, 75.1828, 5, -28, '2021-02-10 17:47:04'),
	(479, 2, 177.788, 22, 'C', 3.91032, 75.1754, 5, -29, '2021-02-10 17:47:11'),
	(480, 2, 177.81, 22, 'C', 3.91554, 75.1853, 5, -28, '2021-02-10 17:47:18'),
	(481, 2, 177.806, 22, 'C', 3.91032, 75.1832, 5, -28, '2021-02-10 17:47:25'),
	(482, 2, 177.807, 22, 'C', 3.91554, 75.184, 5, -29, '2021-02-10 17:47:33'),
	(483, 2, 177.829, 22, 'C', 3.91554, 75.1936, 5, -27, '2021-02-10 17:47:39'),
	(484, 2, 177.817, 22, 'C', 3.91554, 75.1884, 5, -29, '2021-02-10 17:47:46'),
	(485, 2, 177.834, 22, 'C', 3.92075, 75.1957, 5, -29, '2021-02-10 17:47:53'),
	(486, 2, 177.8, 22, 'C', 3.91554, 75.1807, 5, -28, '2021-02-10 17:48:00'),
	(487, 2, 177.793, 22, 'C', 3.91554, 75.1774, 5, -28, '2021-02-10 17:48:07'),
	(488, 2, 177.797, 22, 'C', 3.91032, 75.1792, 5, -27, '2021-02-10 17:48:14'),
	(489, 2, 177.79, 22, 'C', 3.91032, 75.1763, 5, -28, '2021-02-10 17:48:21'),
	(490, 2, 177.821, 22, 'C', 3.91032, 75.19, 5, -28, '2021-02-10 17:48:29'),
	(491, 2, 177.827, 22, 'C', 3.91554, 75.1928, 5, -29, '2021-02-10 17:48:36'),
	(492, 2, 177.817, 22, 'C', 3.91032, 75.1883, 5, -28, '2021-02-10 17:48:43'),
	(493, 2, 177.797, 22, 'C', 3.91032, 75.1792, 5, -26, '2021-02-10 17:48:50'),
	(494, 2, 177.803, 22, 'C', 3.92597, 75.1822, 5, -29, '2021-02-10 17:48:58'),
	(495, 2, 177.801, 22, 'C', 3.91554, 75.181, 5, -29, '2021-02-10 17:49:05'),
	(496, 2, 177.789, 22, 'C', 3.92075, 75.1758, 5, -30, '2021-02-10 17:49:11'),
	(497, 2, 177.792, 22, 'C', 3.91554, 75.1768, 5, -29, '2021-02-10 17:49:18'),
	(498, 2, 177.81, 22, 'C', 3.91554, 75.185, 5, -28, '2021-02-10 17:49:26'),
	(499, 2, 177.813, 22, 'C', 3.91554, 75.1864, 5, -29, '2021-02-10 17:49:33'),
	(500, 2, 177.817, 22, 'C', 3.91554, 75.1882, 5, -29, '2021-02-10 17:49:40'),
	(501, 2, 177.822, 22, 'C', 3.91032, 75.1902, 5, -30, '2021-02-10 17:49:48'),
	(502, 2, 177.803, 22, 'C', 3.91554, 75.182, 5, -30, '2021-02-10 17:49:54'),
	(503, 2, 177.788, 22, 'C', 3.91032, 75.1752, 5, -28, '2021-02-10 17:50:01'),
	(504, 2, 177.821, 22, 'C', 3.91554, 75.1898, 5, -28, '2021-02-10 17:50:08'),
	(505, 2, 177.83, 22, 'C', 3.91032, 75.1939, 5, -29, '2021-02-10 17:50:15'),
	(506, 2, 177.801, 22, 'C', 3.91032, 75.181, 5, -28, '2021-02-10 17:50:22'),
	(507, 2, 177.82, 22, 'C', 3.91032, 75.1894, 5, -30, '2021-02-10 17:50:29'),
	(508, 2, 177.835, 22, 'C', 3.91032, 75.1963, 5, -28, '2021-02-10 17:50:35'),
	(509, 2, 177.826, 22, 'C', 3.91032, 75.1923, 5, -28, '2021-02-10 17:50:43'),
	(510, 2, 177.841, 22, 'C', 3.91032, 75.199, 5, -28, '2021-02-10 17:50:49'),
	(511, 2, 177.823, 22, 'C', 3.91554, 75.1908, 5, -28, '2021-02-10 17:50:56'),
	(512, 2, 177.827, 22, 'C', 3.91554, 75.1929, 5, -26, '2021-02-10 17:51:03'),
	(513, 2, 177.799, 22, 'C', 3.91554, 75.1801, 5, -28, '2021-02-10 17:51:10'),
	(514, 2, 177.824, 22, 'C', 3.91554, 75.1913, 5, -29, '2021-02-10 17:51:16'),
	(515, 2, 177.807, 22, 'C', 3.91554, 75.1839, 5, -28, '2021-02-10 17:51:22'),
	(516, 2, 177.823, 22, 'C', 3.92075, 75.1909, 5, -28, '2021-02-10 17:51:29'),
	(517, 2, 177.798, 22, 'C', 3.91032, 75.1798, 5, -28, '2021-02-10 17:51:36'),
	(518, 2, 177.824, 22, 'C', 3.91554, 75.1911, 5, -28, '2021-02-10 17:51:43'),
	(519, 2, 177.809, 22, 'C', 3.91554, 75.1847, 5, -28, '2021-02-10 17:51:50'),
	(520, 2, 177.829, 22, 'C', 3.91554, 75.1935, 5, -28, '2021-02-10 17:51:57'),
	(521, 2, 177.819, 22, 'C', 3.91554, 75.1891, 5, -28, '2021-02-10 17:52:05'),
	(522, 2, 177.837, 22, 'C', 3.91554, 75.1974, 5, -28, '2021-02-10 17:52:15'),
	(523, 2, 177.843, 22, 'C', 3.91032, 75.1999, 5, -28, '2021-02-10 17:52:21'),
	(524, 2, 177.813, 22, 'C', 3.91032, 75.1863, 5, -28, '2021-02-10 17:52:28'),
	(525, 2, 177.807, 22, 'C', 3.91032, 75.1836, 5, -28, '2021-02-10 17:52:36'),
	(526, 2, 177.822, 22, 'C', 3.91032, 75.1905, 5, -28, '2021-02-10 17:52:43'),
	(527, 2, 177.8, 22, 'C', 3.91554, 75.1806, 5, -29, '2021-02-10 17:52:49'),
	(528, 2, 177.804, 22, 'C', 3.91554, 75.1823, 5, -29, '2021-02-10 17:52:57'),
	(529, 2, 177.814, 22, 'C', 3.91032, 75.1871, 5, -28, '2021-02-10 17:53:04'),
	(530, 2, 177.826, 22, 'C', 3.91554, 75.1924, 5, -27, '2021-02-10 17:53:11'),
	(531, 2, 177.804, 22, 'C', 3.91032, 75.1823, 5, -28, '2021-02-10 17:53:18'),
	(532, 2, 177.786, 22, 'C', 3.91032, 75.1744, 5, -28, '2021-02-10 17:53:26'),
	(533, 2, 177.788, 22, 'C', 3.91032, 75.175, 5, -29, '2021-02-10 17:53:33'),
	(534, 2, 177.812, 22, 'C', 3.91554, 75.1862, 5, -28, '2021-02-10 17:53:40'),
	(535, 2, 177.816, 22, 'C', 3.91554, 75.1877, 5, -29, '2021-02-10 17:53:47'),
	(536, 2, 177.808, 22, 'C', 3.91554, 75.1842, 5, -28, '2021-02-10 17:53:54'),
	(537, 2, 177.803, 22, 'C', 3.91554, 75.1819, 5, -28, '2021-02-10 17:54:01'),
	(538, 2, 177.809, 22, 'C', 3.91554, 75.1848, 5, -28, '2021-02-10 17:54:07'),
	(539, 2, 177.815, 22, 'C', 3.91554, 75.1871, 5, -29, '2021-02-10 17:54:14'),
	(540, 2, 177.833, 22, 'C', 3.92597, 75.1955, 5, -29, '2021-02-10 17:54:25'),
	(541, 2, 177.798, 22, 'C', 3.91554, 75.1799, 5, -26, '2021-02-10 17:54:32'),
	(542, 2, 177.793, 22, 'C', 3.92075, 75.1774, 5, -30, '2021-02-10 17:54:38'),
	(543, 2, 177.794, 22, 'C', 3.91554, 75.1781, 5, -27, '2021-02-10 17:54:45'),
	(544, 2, 177.815, 22, 'C', 3.92075, 75.1872, 5, -29, '2021-02-10 17:54:53'),
	(545, 2, 177.795, 22, 'C', 3.91032, 75.1783, 5, -28, '2021-02-10 17:55:00'),
	(546, 2, 177.802, 22, 'C', 3.91032, 75.1815, 5, -28, '2021-02-10 17:55:07'),
	(547, 2, 177.827, 22, 'C', 3.91032, 75.1925, 5, -28, '2021-02-10 17:55:14'),
	(548, 2, 177.799, 22, 'C', 3.91554, 75.1803, 5, -29, '2021-02-10 17:55:22'),
	(549, 2, 177.813, 22, 'C', 3.91554, 75.1866, 5, -28, '2021-02-10 17:55:28'),
	(550, 2, 177.803, 22, 'C', 3.92075, 75.1819, 5, -28, '2021-02-10 17:55:35'),
	(551, 2, 177.829, 22, 'C', 3.91554, 75.1936, 5, -29, '2021-02-10 17:55:41'),
	(552, 2, 177.798, 22, 'C', 3.91554, 75.1798, 5, -27, '2021-02-10 17:55:48'),
	(553, 2, 177.837, 22, 'C', 3.91032, 75.1969, 5, -28, '2021-02-10 17:55:55'),
	(554, 2, 177.818, 22, 'C', 3.91032, 75.1886, 5, -28, '2021-02-10 17:56:02'),
	(555, 2, 177.809, 22, 'C', 3.91032, 75.1846, 5, -28, '2021-02-10 17:56:10'),
	(556, 2, 177.815, 22, 'C', 3.91032, 75.1875, 5, -29, '2021-02-10 17:56:17'),
	(557, 2, 177.84, 22, 'C', 3.91032, 75.1983, 5, -29, '2021-02-10 17:56:24'),
	(558, 2, 177.809, 22, 'C', 3.91032, 75.1849, 5, -29, '2021-02-10 17:56:31'),
	(559, 2, 177.825, 22, 'C', 3.91554, 75.1918, 5, -27, '2021-02-10 17:56:39'),
	(560, 2, 177.831, 22, 'C', 3.91554, 75.1946, 5, -29, '2021-02-10 17:56:46'),
	(561, 2, 177.823, 22, 'C', 3.91032, 75.1909, 5, -28, '2021-02-10 17:56:53'),
	(562, 2, 177.819, 22, 'C', 3.91032, 75.1893, 5, -29, '2021-02-10 17:57:00'),
	(563, 2, 177.806, 22, 'C', 3.91554, 75.1835, 5, -28, '2021-02-10 17:57:11'),
	(564, 2, 177.838, 22, 'C', 3.91554, 75.1978, 5, -28, '2021-02-10 17:57:21'),
	(565, 2, 177.842, 22, 'C', 3.91554, 75.1994, 5, -29, '2021-02-10 17:57:27'),
	(566, 2, 177.816, 22, 'C', 3.92597, 75.1879, 5, -29, '2021-02-10 17:57:33'),
	(567, 2, 177.807, 22, 'C', 3.91032, 75.1838, 5, -29, '2021-02-10 17:57:41'),
	(568, 2, 177.812, 22, 'C', 3.91032, 75.1858, 5, -29, '2021-02-10 17:57:48'),
	(569, 2, 177.796, 22, 'C', 3.91032, 75.1786, 5, -29, '2021-02-10 17:57:55'),
	(570, 2, 177.79, 22, 'C', 3.91554, 75.1759, 5, -29, '2021-02-10 17:58:01'),
	(571, 2, 177.831, 22, 'C', 3.91554, 75.1943, 5, -28, '2021-02-10 17:58:09'),
	(572, 2, 177.838, 22, 'C', 3.91554, 75.1974, 5, -28, '2021-02-10 17:58:16'),
	(573, 2, 177.809, 22, 'C', 3.91032, 75.1844, 5, -29, '2021-02-10 17:58:23'),
	(574, 2, 177.818, 22, 'C', 3.91554, 75.1885, 5, -29, '2021-02-10 17:58:30'),
	(575, 2, 177.824, 22, 'C', 3.91554, 75.1914, 5, -28, '2021-02-10 17:58:37'),
	(576, 2, 177.804, 22, 'C', 3.91554, 75.1825, 5, -29, '2021-02-10 17:58:44'),
	(577, 2, 177.793, 22, 'C', 3.91554, 75.1775, 5, -28, '2021-02-10 17:58:51'),
	(578, 2, 177.828, 22, 'C', 3.91032, 75.1931, 5, -29, '2021-02-10 17:58:57'),
	(579, 2, 177.862, 22, 'C', 3.91032, 75.2082, 5, -28, '2021-02-10 17:59:05'),
	(580, 2, 177.831, 22, 'C', 3.91032, 75.1943, 5, -27, '2021-02-10 17:59:12'),
	(581, 2, 177.823, 22, 'C', 3.91554, 75.1909, 5, -28, '2021-02-10 17:59:19'),
	(582, 2, 177.811, 22, 'C', 3.91554, 75.1857, 5, -28, '2021-02-10 17:59:25'),
	(583, 2, 177.853, 22, 'C', 3.91032, 75.2044, 5, -29, '2021-02-10 17:59:33'),
	(584, 2, 177.821, 22, 'C', 3.91554, 75.19, 5, -30, '2021-02-10 17:59:40'),
	(585, 2, 177.813, 22, 'C', 3.91554, 75.1862, 5, -28, '2021-02-10 17:59:47'),
	(586, 2, 177.839, 22, 'C', 3.91554, 75.1981, 5, -29, '2021-02-10 17:59:55'),
	(587, 2, 177.824, 22, 'C', 3.91554, 75.1914, 5, -28, '2021-02-10 18:00:02'),
	(588, 2, 177.811, 22, 'C', 3.91032, 75.1858, 5, -28, '2021-02-10 18:00:09'),
	(589, 2, 177.829, 22, 'C', 3.91554, 75.1937, 5, -27, '2021-02-10 18:00:16'),
	(590, 2, 177.817, 22, 'C', 3.91032, 75.1881, 5, -26, '2021-02-10 18:00:24'),
	(591, 2, 177.85, 22, 'C', 3.91032, 75.203, 5, -27, '2021-02-10 18:00:31'),
	(592, 2, 177.822, 22, 'C', 3.91032, 75.1903, 5, -27, '2021-02-10 18:00:38'),
	(593, 2, 177.809, 22, 'C', 3.91032, 75.1848, 5, -28, '2021-02-10 18:00:48'),
	(594, 2, 177.82, 22, 'C', 3.91554, 75.1895, 5, -28, '2021-02-10 18:00:55'),
	(595, 2, 177.805, 22, 'C', 3.91032, 75.1831, 5, -27, '2021-02-10 18:01:02'),
	(596, 2, 177.832, 22, 'C', 3.91032, 75.1949, 5, -27, '2021-02-10 18:01:09'),
	(597, 2, 177.843, 22, 'C', 3.91554, 75.1997, 5, -27, '2021-02-10 18:01:16'),
	(598, 2, 177.827, 22, 'C', 3.91554, 75.1927, 5, -27, '2021-02-10 18:01:24'),
	(599, 2, 177.816, 22, 'C', 3.91554, 75.1876, 5, -27, '2021-02-10 18:01:31'),
	(600, 2, 177.815, 22, 'C', 3.91032, 75.1872, 5, -27, '2021-02-10 18:01:38'),
	(601, 2, 177.824, 22, 'C', 3.91032, 75.1914, 5, -27, '2021-02-10 18:01:45'),
	(602, 2, 177.812, 22, 'C', 3.91554, 75.1862, 5, -27, '2021-02-10 18:01:52'),
	(603, 2, 177.817, 22, 'C', 3.91032, 75.1882, 5, -27, '2021-02-10 18:01:59'),
	(604, 2, 177.807, 22, 'C', 3.91032, 75.1836, 5, -27, '2021-02-10 18:02:06'),
	(605, 2, 177.817, 22, 'C', 3.91032, 75.1883, 5, -27, '2021-02-10 18:02:13'),
	(606, 2, 177.845, 22, 'C', 3.91554, 75.2008, 5, -28, '2021-02-10 18:02:21'),
	(607, 2, 177.807, 22, 'C', 3.91032, 75.1838, 5, -28, '2021-02-10 18:02:28'),
	(608, 2, 177.815, 22, 'C', 3.91032, 75.1874, 5, -27, '2021-02-10 18:02:35'),
	(609, 2, 177.819, 22, 'C', 3.91032, 75.1893, 5, -27, '2021-02-10 18:02:42'),
	(610, 2, 177.809, 22, 'C', 3.91554, 75.1844, 5, -27, '2021-02-10 18:02:50'),
	(611, 2, 177.804, 22, 'C', 3.91032, 75.1823, 5, -27, '2021-02-10 18:02:57'),
	(612, 2, 177.801, 22, 'C', 3.91032, 75.1809, 5, -28, '2021-02-10 18:03:04'),
	(613, 2, 177.85, 22, 'C', 3.91032, 75.2028, 5, -26, '2021-02-10 18:03:10'),
	(614, 2, 177.811, 22, 'C', 3.91032, 75.1857, 5, -28, '2021-02-10 18:03:17'),
	(615, 2, 177.822, 22, 'C', 3.91032, 75.1902, 5, -28, '2021-02-10 18:03:23'),
	(616, 2, 177.824, 22, 'C', 3.91032, 75.1913, 5, -27, '2021-02-10 18:03:30'),
	(617, 2, 177.821, 22, 'C', 3.91032, 75.19, 5, -27, '2021-02-10 18:03:37'),
	(618, 2, 177.825, 22, 'C', 3.91554, 75.1917, 5, -28, '2021-02-10 18:03:48'),
	(619, 2, 177.852, 22, 'C', 3.91554, 75.2039, 5, -28, '2021-02-10 18:03:55'),
	(620, 2, 177.838, 22, 'C', 3.91554, 75.1976, 5, -27, '2021-02-10 18:04:02'),
	(621, 2, 177.828, 22, 'C', 3.91554, 75.1933, 5, -27, '2021-02-10 18:04:09'),
	(622, 2, 177.811, 22, 'C', 3.91032, 75.1857, 5, -28, '2021-02-10 18:04:17'),
	(623, 2, 177.812, 22, 'C', 3.92075, 75.1861, 5, -27, '2021-02-10 18:04:23'),
	(624, 2, 177.819, 22, 'C', 3.91554, 75.1891, 5, -27, '2021-02-10 18:04:30'),
	(625, 2, 177.825, 22, 'C', 3.91032, 75.1916, 5, -27, '2021-02-10 18:04:36'),
	(626, 2, 177.817, 22, 'C', 3.91032, 75.1884, 5, -27, '2021-02-10 18:04:44'),
	(627, 2, 177.823, 22, 'C', 3.91032, 75.1909, 5, -27, '2021-02-10 18:04:51'),
	(628, 2, 177.827, 22, 'C', 3.91032, 75.1927, 5, -27, '2021-02-10 18:04:58'),
	(629, 2, 177.844, 22, 'C', 3.91032, 75.2001, 5, -28, '2021-02-10 18:05:05'),
	(630, 2, 177.85, 22, 'C', 3.91554, 75.203, 5, -27, '2021-02-10 18:05:12'),
	(631, 2, 177.813, 22, 'C', 3.91554, 75.1866, 5, -28, '2021-02-10 18:05:19'),
	(632, 2, 177.839, 22, 'C', 3.92075, 75.1979, 5, -29, '2021-02-10 18:05:25'),
	(633, 2, 177.827, 22, 'C', 3.91554, 75.1928, 5, -28, '2021-02-10 18:05:31'),
	(634, 2, 177.84, 22, 'C', 3.91032, 75.1985, 5, -28, '2021-02-10 18:05:39'),
	(635, 2, 177.818, 22, 'C', 3.91032, 75.1887, 5, -28, '2021-02-10 18:05:46'),
	(636, 2, 177.842, 22, 'C', 3.91554, 75.1995, 5, -28, '2021-02-10 18:05:52'),
	(637, 2, 177.819, 22, 'C', 3.91032, 75.1891, 5, -28, '2021-02-10 18:06:00'),
	(638, 2, 177.851, 22, 'C', 3.92597, 75.2035, 5, -29, '2021-02-10 18:06:06'),
	(639, 2, 177.837, 22, 'C', 3.92075, 75.1971, 5, -29, '2021-02-10 18:06:12'),
	(640, 2, 177.85, 22, 'C', 3.91032, 75.2029, 5, -28, '2021-02-10 18:06:19'),
	(641, 2, 177.837, 22, 'C', 3.92075, 75.197, 5, -27, '2021-02-10 18:06:26'),
	(642, 2, 177.839, 22, 'C', 3.91032, 75.198, 5, -28, '2021-02-10 18:06:33'),
	(643, 2, 177.806, 22, 'C', 3.91032, 75.1834, 5, -28, '2021-02-10 18:06:40'),
	(644, 2, 177.841, 22, 'C', 3.91032, 75.1989, 5, -29, '2021-02-10 18:06:46'),
	(645, 2, 177.84, 22, 'C', 3.91554, 75.1986, 5, -28, '2021-02-10 18:06:54'),
	(646, 2, 35.9363, 21, 'C', 3.8269, 5.56217, 5, -38, '2021-02-10 18:57:35'),
	(647, 2, 55.031, 21, 'C', 3.84776, 15.6595, 5, -40, '2021-02-10 18:57:43'),
	(648, 2, 80.8378, 21, 'C', 3.85297, 28.9469, 5, -36, '2021-02-10 18:57:50'),
	(649, 2, 22.4079, 21, 'C', 3.85297, -1.72845, 5, -47, '2021-02-10 18:57:57'),
	(650, 2, 13.5524, 21, 'C', 3.85297, -6.56224, 5, -37, '2021-02-10 18:58:04'),
	(651, 2, 50.4672, 21, 'C', 3.85819, 13.2667, 5, -42, '2021-02-10 18:58:12'),
	(652, 2, 29.4114, 21, 'C', 3.85297, 2.06, 5, -36, '2021-02-10 18:58:18'),
	(653, 2, 18.1605, 21, 'C', 3.80605, -4.04083, 5, -35, '2021-02-10 18:58:25'),
	(654, 2, 80.6476, 23, 'C', 3.86861, 28.8505, 1, -49, '2021-02-15 20:48:24'),
	(655, 2, 82.9415, 23, 'C', 3.86861, 30.0118, 5, -51, '2021-02-15 20:48:27'),
	(656, 2, 64.6882, 23, 'C', 3.8634, 20.6801, 5, -58, '2021-02-15 20:48:33'),
	(657, 2, 53.2143, 23, 'C', 3.86861, 14.7085, 5, -46, '2021-02-15 20:48:39'),
	(658, 2, 58.4609, 23, 'C', 3.87904, 17.4492, 5, -53, '2021-02-15 20:48:46'),
	(659, 2, 38.2659, 23, 'C', 3.87904, 6.80621, 5, -54, '2021-02-15 20:48:52');
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
	(1, 'Kerjox', '111111111', '$2y$10$cU.9SseE4nD18djgbCLx8OqiVMBztjTh37VdRNBpOFxQXk3yrFKMm', 'alcachofa@gmail.com');
/*!40000 ALTER TABLE `users` ENABLE KEYS */;

-- Volcando estructura para disparador cervecero.placas_AU
SET @OLDTMP_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION';
DELIMITER //
CREATE TRIGGER `placas_AU` AFTER UPDATE ON `placas` FOR EACH ROW BEGIN
INSERT INTO log_Placas_Firmwares (id_Placa, oldVersion, newVersion) VALUES (new.id_Placa, OLD.firmware, NEW.firmware);
END//
DELIMITER ;
SET SQL_MODE=@OLDTMP_SQL_MODE;

-- Volcando estructura para disparador cervecero.users_AI
SET @OLDTMP_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION';
DELIMITER //
CREATE TRIGGER `users_AI` AFTER INSERT ON `users` FOR EACH ROW BEGIN
INSERT INTO actual_Process SET id_User = NEW.id_User;
END//
DELIMITER ;
SET SQL_MODE=@OLDTMP_SQL_MODE;

-- Volcando estructura para disparador cervecero.users_BD
SET @OLDTMP_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION';
DELIMITER //
CREATE TRIGGER `users_BD` BEFORE DELETE ON `users` FOR EACH ROW BEGIN
DELETE FROM actual_Process WHERE id_User = OLD.id_User;
END//
DELIMITER ;
SET SQL_MODE=@OLDTMP_SQL_MODE;

/*!40101 SET SQL_MODE=IFNULL(@OLD_SQL_MODE, '') */;
/*!40014 SET FOREIGN_KEY_CHECKS=IFNULL(@OLD_FOREIGN_KEY_CHECKS, 1) */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40111 SET SQL_NOTES=IFNULL(@OLD_SQL_NOTES, 1) */;
