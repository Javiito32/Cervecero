-- --------------------------------------------------------
-- Host:                         127.0.0.1
-- Versión del servidor:         10.4.11-MariaDB - mariadb.org binary distribution
-- SO del servidor:              Win64
-- HeidiSQL Versión:             11.0.0.5919
-- --------------------------------------------------------

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!50503 SET NAMES utf8mb4 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;


-- Volcando estructura de base de datos para cervecero
CREATE DATABASE IF NOT EXISTS `cervecero` /*!40100 DEFAULT CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci */;
USE `cervecero`;

-- Volcando estructura para tabla cervecero.estados
CREATE TABLE IF NOT EXISTS `estados` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `estado` varchar(50) CHARACTER SET utf8mb4 DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=4 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;

-- Volcando datos para la tabla cervecero.estados: ~3 rows (aproximadamente)
/*!40000 ALTER TABLE `estados` DISABLE KEYS */;
REPLACE INTO `estados` (`id`, `estado`) VALUES
	(1, 'Iniciado'),
	(2, 'Finalizado'),
	(3, 'Fallo');
/*!40000 ALTER TABLE `estados` ENABLE KEYS */;

-- Volcando estructura para tabla cervecero.info
CREATE TABLE IF NOT EXISTS `info` (
  `IDplaca` int(11) NOT NULL AUTO_INCREMENT,
  `receta` int(11) DEFAULT NULL,
  `proceso` int(11) DEFAULT NULL,
  `pasoProceso` int(11) DEFAULT NULL,
  `estado` int(11) DEFAULT NULL,
  `tiempoRestante` int(11) DEFAULT NULL,
  `porcentaje` tinyint(4) DEFAULT NULL,
  PRIMARY KEY (`IDplaca`),
  KEY `FK_info_recetas` (`receta`),
  KEY `FK_info_procesos` (`proceso`),
  KEY `FK_info_estados` (`estado`),
  CONSTRAINT `FK_info_estados` FOREIGN KEY (`estado`) REFERENCES `estados` (`id`) ON DELETE NO ACTION ON UPDATE CASCADE,
  CONSTRAINT `FK_info_placas` FOREIGN KEY (`IDplaca`) REFERENCES `placas` (`id`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `FK_info_procesos` FOREIGN KEY (`proceso`) REFERENCES `procesos` (`id`) ON DELETE NO ACTION ON UPDATE CASCADE,
  CONSTRAINT `FK_info_recetas` FOREIGN KEY (`receta`) REFERENCES `recetas` (`ID`) ON DELETE NO ACTION ON UPDATE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=13 DEFAULT CHARSET=utf8mb4;

-- Volcando datos para la tabla cervecero.info: ~1 rows (aproximadamente)
/*!40000 ALTER TABLE `info` DISABLE KEYS */;
REPLACE INTO `info` (`IDplaca`, `receta`, `proceso`, `pasoProceso`, `estado`, `tiempoRestante`, `porcentaje`) VALUES
	(1, 1, 1, 1, 2, 0, 100);
/*!40000 ALTER TABLE `info` ENABLE KEYS */;

-- Volcando estructura para tabla cervecero.log
CREATE TABLE IF NOT EXISTS `log` (
  `ID` int(11) NOT NULL AUTO_INCREMENT,
  `IDplaca` int(11) DEFAULT NULL,
  `receta` int(11) DEFAULT NULL,
  `time` timestamp NOT NULL DEFAULT current_timestamp() ON UPDATE current_timestamp(),
  `proceso` int(11) DEFAULT NULL,
  `pasoProceso` int(11) DEFAULT NULL,
  `estado` int(11) DEFAULT NULL,
  `tiempoRestante` int(11) DEFAULT NULL,
  `porcentaje` int(11) DEFAULT NULL,
  PRIMARY KEY (`ID`),
  KEY `estado` (`estado`),
  KEY `proceso` (`proceso`),
  KEY `placaID` (`IDplaca`) USING BTREE,
  CONSTRAINT `FK_log_estados` FOREIGN KEY (`estado`) REFERENCES `estados` (`id`) ON DELETE NO ACTION ON UPDATE CASCADE,
  CONSTRAINT `FK_log_placas` FOREIGN KEY (`IDplaca`) REFERENCES `placas` (`id`) ON DELETE NO ACTION ON UPDATE CASCADE,
  CONSTRAINT `FK_log_procesos` FOREIGN KEY (`proceso`) REFERENCES `procesos` (`id`) ON DELETE NO ACTION ON UPDATE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=166 DEFAULT CHARSET=utf8mb4;

-- Volcando datos para la tabla cervecero.log: ~61 rows (aproximadamente)
/*!40000 ALTER TABLE `log` DISABLE KEYS */;
REPLACE INTO `log` (`ID`, `IDplaca`, `receta`, `time`, `proceso`, `pasoProceso`, `estado`, `tiempoRestante`, `porcentaje`) VALUES
	(6, 1, NULL, '2020-03-20 19:53:53', 1, NULL, 1, NULL, NULL),
	(7, 1, NULL, '2020-03-20 21:13:26', 1, NULL, 1, NULL, NULL),
	(8, 1, NULL, '2020-03-20 21:14:23', 1, NULL, 3, NULL, NULL),
	(9, 1, NULL, '2020-03-21 16:11:11', 1, NULL, 1, NULL, NULL),
	(10, 1, NULL, '2020-03-21 16:11:12', 1, NULL, 2, NULL, NULL),
	(11, 1, NULL, '2020-03-21 16:12:41', 1, NULL, 1, NULL, NULL),
	(12, 1, NULL, '2020-03-21 16:13:48', 1, NULL, 2, NULL, NULL),
	(13, 1, NULL, '2020-03-21 17:51:33', 1, NULL, 1, NULL, NULL),
	(14, 1, NULL, '2020-03-21 17:52:33', 1, NULL, 2, NULL, NULL),
	(15, 1, NULL, '2020-03-21 17:56:53', 1, NULL, 1, NULL, NULL),
	(16, 1, NULL, '2020-03-21 17:57:56', 1, NULL, 2, NULL, NULL),
	(17, 1, NULL, '2020-03-21 21:23:16', 1, NULL, 1, NULL, NULL),
	(18, 1, NULL, '2020-03-21 21:24:19', 1, NULL, 2, NULL, NULL),
	(19, 1, NULL, '2020-03-21 21:24:38', 1, NULL, 1, NULL, NULL),
	(20, 1, NULL, '2020-03-21 21:25:42', 1, NULL, 2, NULL, NULL),
	(21, 1, NULL, '2020-03-21 21:25:52', 1, NULL, 1, NULL, NULL),
	(22, 1, NULL, '2020-03-21 21:26:00', 1, NULL, 3, NULL, NULL),
	(23, 1, NULL, '2020-03-21 21:27:50', 1, NULL, 1, NULL, NULL),
	(24, 1, NULL, '2020-03-21 21:27:58', 1, NULL, 3, NULL, NULL),
	(25, 1, NULL, '2020-03-21 22:25:08', 1, NULL, 1, NULL, NULL),
	(26, 1, NULL, '2020-03-21 22:25:26', 1, NULL, 3, NULL, NULL),
	(27, 1, NULL, '2020-03-21 22:32:10', 4, NULL, 1, NULL, NULL),
	(28, 1, NULL, '2020-03-21 22:33:27', 4, NULL, 3, NULL, NULL),
	(29, 1, NULL, '2020-03-21 22:33:43', 4, NULL, 1, NULL, NULL),
	(30, 1, NULL, '2020-03-21 22:34:50', 4, NULL, 3, NULL, NULL),
	(31, 1, NULL, '2020-03-21 22:35:05', 4, NULL, 1, NULL, NULL),
	(32, 1, NULL, '2020-03-21 22:39:31', 4, NULL, 1, NULL, NULL),
	(33, 1, NULL, '2020-03-21 22:39:42', 4, NULL, 3, NULL, NULL),
	(34, 1, NULL, '2020-03-21 22:52:58', 4, NULL, 1, NULL, NULL),
	(35, 1, NULL, '2020-03-21 22:56:50', 4, NULL, 3, NULL, NULL),
	(36, 1, NULL, '2020-03-21 23:06:50', 4, NULL, 1, NULL, NULL),
	(37, 1, NULL, '2020-03-21 23:07:23', 4, NULL, 3, NULL, NULL),
	(38, 1, NULL, '2020-03-21 23:13:32', 4, NULL, 1, NULL, NULL),
	(39, 1, NULL, '2020-03-21 23:17:41', 4, NULL, 1, NULL, NULL),
	(40, 1, NULL, '2020-03-21 23:24:26', 4, NULL, 1, NULL, NULL),
	(41, 1, NULL, '2020-03-21 23:28:10', 4, NULL, 1, NULL, NULL),
	(42, 1, NULL, '2020-03-22 00:44:12', 1, NULL, 1, NULL, NULL),
	(43, 1, NULL, '2020-03-22 00:47:08', 1, NULL, 1, NULL, NULL),
	(44, 1, NULL, '2020-03-22 00:48:03', 1, NULL, 1, NULL, NULL),
	(45, 1, NULL, '2020-03-22 00:49:31', 1, NULL, 1, NULL, NULL),
	(46, 1, NULL, '2020-03-22 00:51:04', 1, NULL, 1, NULL, NULL),
	(47, 1, NULL, '2020-03-22 00:52:42', 1, NULL, 1, NULL, NULL),
	(48, 1, NULL, '2020-03-22 00:59:19', 1, NULL, 1, NULL, NULL),
	(49, 1, NULL, '2020-03-22 00:59:56', 1, NULL, 3, NULL, NULL),
	(50, 1, NULL, '2020-03-22 01:01:34', 1, NULL, 1, NULL, NULL),
	(51, 1, NULL, '2020-03-22 01:08:12', 1, NULL, 1, NULL, NULL),
	(52, 1, NULL, '2020-03-22 01:12:27', 1, NULL, 1, NULL, NULL),
	(53, 1, NULL, '2020-03-22 01:13:30', 1, NULL, 2, NULL, NULL),
	(54, 1, NULL, '2020-03-22 01:16:18', 1, NULL, 1, NULL, NULL),
	(55, 1, NULL, '2020-03-22 01:17:22', 1, NULL, 2, NULL, NULL),
	(56, 1, NULL, '2020-03-22 01:17:48', 1, NULL, 1, NULL, NULL),
	(57, 1, NULL, '2020-03-22 01:17:52', 1, NULL, 2, NULL, NULL),
	(58, 1, NULL, '2020-03-22 01:18:23', 1, NULL, 1, NULL, NULL),
	(59, 1, NULL, '2020-03-22 01:20:18', 1, NULL, 1, NULL, NULL),
	(60, 1, NULL, '2020-03-22 01:20:25', 1, NULL, 2, NULL, NULL),
	(61, 1, NULL, '2020-03-22 01:21:26', 1, NULL, 2, NULL, NULL),
	(64, 1, NULL, '2020-03-22 14:21:00', 1, 1, 1, NULL, NULL),
	(65, 1, NULL, '2020-03-22 14:26:04', 1, 1, 2, NULL, NULL),
	(66, 1, NULL, '2020-03-22 20:16:26', 1, 1, 1, NULL, 0),
	(67, 1, NULL, '2020-03-22 20:20:20', 1, 1, 1, NULL, 0),
	(68, 1, NULL, '2020-03-23 02:08:14', 1, 1, 1, NULL, 0),
	(69, 1, NULL, '2020-03-23 02:09:06', 1, 1, 1, 100, 0),
	(70, 1, 1, '2020-03-23 02:27:46', 1, 1, 1, 120, 0),
	(71, 1, 1, '2020-03-23 13:31:05', 1, 1, 2, 120, 96),
	(72, 1, 1, '2020-03-23 13:38:32', 1, 1, 1, 120, 96),
	(73, 1, 1, '2020-03-23 19:32:02', 1, 1, 1, 30, 96),
	(74, 1, 1, '2020-03-24 20:25:44', 1, 1, 1, 30, 50),
	(79, 1, 1, '2020-03-24 21:21:25', 1, 1, 2, 0, 100),
	(80, 1, 1, '2020-03-26 19:32:49', 1, 1, 1, 0, 0),
	(81, 1, 1, '2020-03-26 19:32:57', 1, 1, 1, 55, 6),
	(82, 1, 1, '2020-03-26 19:33:01', 1, 1, 1, 50, 16),
	(83, 1, 1, '2020-03-26 19:33:06', 1, 1, 1, 45, 25),
	(84, 1, 1, '2020-03-26 19:33:11', 1, 1, 1, 40, 33),
	(85, 1, 1, '2020-03-26 19:33:17', 1, 1, 1, 35, 40),
	(86, 1, 1, '2020-03-26 19:33:21', 1, 1, 1, 30, 50),
	(87, 1, 1, '2020-03-26 19:50:15', 1, 1, 1, 25, 56),
	(88, 1, 1, '2020-03-26 19:50:20', 1, 1, 1, 20, 66),
	(89, 1, 1, '2020-03-26 19:50:24', 1, 1, 1, 15, 73),
	(90, 1, 1, '2020-03-26 19:50:30', 1, 1, 1, 10, 81),
	(91, 1, 1, '2020-03-26 19:50:34', 1, 1, 1, 5, 91),
	(92, 1, 1, '2020-03-26 19:50:52', 1, 1, 2, 0, 100),
	(93, 1, 1, '2020-03-26 19:51:57', 1, 2, 1, 0, 0),
	(94, 1, 1, '2020-03-26 19:52:05', 1, 2, 1, 295, 1),
	(95, 1, 1, '2020-03-26 19:52:11', 1, 2, 1, 290, 3),
	(96, 1, 1, '2020-03-26 19:52:15', 1, 2, 1, 285, 5),
	(97, 1, 1, '2020-03-26 19:52:20', 1, 2, 1, 280, 6),
	(98, 1, 1, '2020-03-26 19:52:27', 1, 2, 1, 275, 8),
	(99, 1, 1, '2020-03-26 19:52:34', 1, 2, 1, 270, 9),
	(100, 1, 1, '2020-03-26 19:52:36', 1, 2, 1, 264, 12),
	(101, 1, 1, '2020-03-26 19:52:41', 1, 2, 1, 259, 13),
	(102, 1, 1, '2020-03-26 19:52:46', 1, 2, 1, 254, 15),
	(103, 1, 1, '2020-03-26 19:52:52', 1, 2, 1, 249, 16),
	(104, 1, 1, '2020-03-26 19:52:56', 1, 2, 1, 244, 18),
	(105, 1, 1, '2020-03-26 19:53:01', 1, 2, 1, 239, 20),
	(106, 1, 1, '2020-03-26 19:53:06', 1, 2, 1, 234, 22),
	(107, 1, 1, '2020-03-26 19:53:12', 1, 2, 1, 229, 23),
	(108, 1, 1, '2020-03-26 19:53:17', 1, 2, 1, 224, 25),
	(109, 1, 1, '2020-03-26 19:53:21', 1, 2, 1, 219, 27),
	(110, 1, 1, '2020-03-26 19:53:26', 1, 2, 1, 214, 28),
	(111, 1, 1, '2020-03-26 19:53:40', 1, 2, 1, 209, 30),
	(112, 1, 1, '2020-03-26 19:53:47', 1, 2, 1, 204, 32),
	(113, 1, 1, '2020-03-26 19:53:50', 1, 2, 1, 199, 33),
	(114, 1, 1, '2020-03-26 19:54:35', 1, 2, 1, 194, 35),
	(115, 1, 1, '2020-03-26 19:54:40', 1, 2, 1, 189, 37),
	(116, 1, 1, '2020-03-26 19:54:45', 1, 2, 1, 184, 38),
	(117, 1, 1, '2020-03-26 19:54:51', 1, 2, 1, 179, 40),
	(118, 1, 1, '2020-03-26 19:54:55', 1, 2, 1, 174, 42),
	(119, 1, 1, '2020-03-26 19:55:00', 1, 2, 1, 169, 43),
	(120, 1, 1, '2020-03-26 19:55:05', 1, 2, 1, 164, 45),
	(121, 1, 1, '2020-03-26 19:55:11', 1, 2, 1, 159, 46),
	(122, 1, 1, '2020-03-26 19:55:16', 1, 2, 1, 154, 48),
	(123, 1, 1, '2020-03-26 19:55:20', 1, 2, 1, 149, 50),
	(124, 1, 1, '2020-03-26 19:55:25', 1, 2, 1, 144, 52),
	(125, 1, 1, '2020-03-26 19:55:30', 1, 2, 1, 139, 53),
	(126, 1, 1, '2020-03-26 19:55:35', 1, 2, 1, 134, 55),
	(127, 1, 1, '2020-03-26 19:55:40', 1, 2, 1, 129, 57),
	(128, 1, 1, '2020-03-26 19:55:45', 1, 2, 1, 124, 58),
	(129, 1, 1, '2020-03-26 19:56:29', 1, 2, 1, 119, 60),
	(130, 1, 1, '2020-03-26 19:56:35', 1, 2, 1, 114, 61),
	(131, 1, 1, '2020-03-26 19:56:39', 1, 2, 1, 109, 63),
	(132, 1, 1, '2020-03-26 19:56:44', 1, 2, 1, 104, 65),
	(133, 1, 1, '2020-03-26 19:56:49', 1, 2, 1, 99, 67),
	(134, 1, 1, '2020-03-26 19:56:55', 1, 2, 1, 94, 68),
	(135, 1, 1, '2020-03-26 19:56:59', 1, 2, 1, 89, 70),
	(136, 1, 1, '2020-03-26 19:57:04', 1, 2, 1, 84, 72),
	(137, 1, 1, '2020-03-26 19:57:18', 1, 2, 1, 79, 73),
	(138, 1, 1, '2020-03-26 19:57:21', 1, 2, 1, 68, 77),
	(139, 1, 1, '2020-03-26 19:57:25', 1, 2, 1, 63, 79),
	(140, 1, 1, '2020-03-26 19:57:32', 1, 2, 1, 58, 80),
	(141, 1, 1, '2020-03-26 19:57:35', 1, 2, 1, 53, 82),
	(142, 1, 1, '2020-03-26 19:57:40', 1, 2, 1, 48, 84),
	(143, 1, 1, '2020-03-26 19:57:45', 1, 2, 1, 43, 85),
	(144, 1, 1, '2020-03-26 19:57:51', 1, 2, 1, 38, 87),
	(145, 1, 1, '2020-03-26 19:57:55', 1, 2, 1, 33, 89),
	(146, 1, 1, '2020-03-26 19:58:00', 1, 2, 1, 28, 90),
	(147, 1, 1, '2020-03-26 19:58:05', 1, 2, 1, 23, 92),
	(148, 1, 1, '2020-03-26 19:58:10', 1, 2, 1, 18, 94),
	(149, 1, 1, '2020-03-26 19:58:16', 1, 2, 1, 13, 95),
	(150, 1, 1, '2020-03-26 19:58:29', 1, 2, 1, 8, 97),
	(151, 1, 1, '2020-03-26 19:58:34', 1, 2, 1, 3, 99),
	(152, 1, 1, '2020-03-26 19:58:38', 1, 2, 2, 0, 100),
	(153, 1, 1, '2020-03-26 19:58:57', 1, 1, 1, 0, 0),
	(154, 1, 1, '2020-03-26 19:59:05', 1, 1, 1, 55, 6),
	(155, 1, 1, '2020-03-26 19:59:10', 1, 1, 1, 50, 16),
	(156, 1, 1, '2020-03-26 19:59:16', 1, 1, 1, 45, 23),
	(157, 1, 1, '2020-03-26 19:59:20', 1, 1, 1, 40, 33),
	(158, 1, 1, '2020-03-26 19:59:25', 1, 1, 1, 35, 41),
	(159, 1, 1, '2020-03-26 19:59:30', 1, 1, 1, 30, 50),
	(160, 1, 1, '2020-03-26 19:59:36', 1, 1, 1, 25, 56),
	(161, 1, 1, '2020-03-26 19:59:51', 1, 1, 1, 20, 65),
	(162, 1, 1, '2020-03-26 19:59:55', 1, 1, 1, 15, 73),
	(163, 1, 1, '2020-03-26 20:00:01', 1, 1, 1, 10, 81),
	(164, 1, 1, '2020-03-26 20:00:05', 1, 1, 1, 5, 91),
	(165, 1, 1, '2020-03-26 20:00:11', 1, 1, 2, 0, 100);
/*!40000 ALTER TABLE `log` ENABLE KEYS */;

-- Volcando estructura para tabla cervecero.menu
CREATE TABLE IF NOT EXISTS `menu` (
  `IDplaca` int(11) NOT NULL DEFAULT 0,
  `menu` int(11) NOT NULL DEFAULT 0,
  `fallo` int(11) NOT NULL DEFAULT 0,
  PRIMARY KEY (`IDplaca`) USING BTREE,
  CONSTRAINT `FK_menu_placas` FOREIGN KEY (`IDplaca`) REFERENCES `placas` (`id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

-- Volcando datos para la tabla cervecero.menu: ~2 rows (aproximadamente)
/*!40000 ALTER TABLE `menu` DISABLE KEYS */;
REPLACE INTO `menu` (`IDplaca`, `menu`, `fallo`) VALUES
	(1, 0, 0);
/*!40000 ALTER TABLE `menu` ENABLE KEYS */;

-- Volcando estructura para tabla cervecero.placas
CREATE TABLE IF NOT EXISTS `placas` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `mac` varchar(17) CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_520_ci NOT NULL,
  `usuario` int(11) DEFAULT NULL,
  PRIMARY KEY (`id`) USING BTREE
) ENGINE=InnoDB AUTO_INCREMENT=13 DEFAULT CHARSET=utf8mb4;

-- Volcando datos para la tabla cervecero.placas: ~1 rows (aproximadamente)
/*!40000 ALTER TABLE `placas` DISABLE KEYS */;
REPLACE INTO `placas` (`id`, `mac`, `usuario`) VALUES
	(1, '84:0D:8E:B0:7C:53', NULL);
/*!40000 ALTER TABLE `placas` ENABLE KEYS */;

-- Volcando estructura para tabla cervecero.procesos
CREATE TABLE IF NOT EXISTS `procesos` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `proceso` varchar(17) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=5 DEFAULT CHARSET=utf8mb4;

-- Volcando datos para la tabla cervecero.procesos: ~4 rows (aproximadamente)
/*!40000 ALTER TABLE `procesos` DISABLE KEYS */;
REPLACE INTO `procesos` (`id`, `proceso`) VALUES
	(1, 'Maceracion'),
	(2, 'Coccion'),
	(3, 'Transvase'),
	(4, 'Fermentacion');
/*!40000 ALTER TABLE `procesos` ENABLE KEYS */;

-- Volcando estructura para tabla cervecero.recetas
CREATE TABLE IF NOT EXISTS `recetas` (
  `ID` int(11) NOT NULL AUTO_INCREMENT,
  `nombre` varchar(50) NOT NULL DEFAULT '0',
  `tempMacer` varchar(50) NOT NULL DEFAULT '0',
  `tiempoMacer` varchar(50) NOT NULL DEFAULT '0',
  `tempCoc` varchar(50) NOT NULL DEFAULT '0',
  `tiempoCoc` varchar(50) NOT NULL DEFAULT '0',
  `tempFermen` varchar(50) NOT NULL DEFAULT '0',
  `tiempoFermen` varchar(50) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=utf8mb4;

-- Volcando datos para la tabla cervecero.recetas: ~0 rows (aproximadamente)
/*!40000 ALTER TABLE `recetas` DISABLE KEYS */;
REPLACE INTO `recetas` (`ID`, `nombre`, `tempMacer`, `tiempoMacer`, `tempCoc`, `tiempoCoc`, `tempFermen`, `tiempoFermen`) VALUES
	(1, 'Pinta', '0:54:78:89:34', '0:1:5:7:3', '0:30', '0:1', '0:42', '0:3');
/*!40000 ALTER TABLE `recetas` ENABLE KEYS */;

-- Volcando estructura para disparador cervecero.info_AU
SET @OLDTMP_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_ZERO_IN_DATE,NO_ZERO_DATE,NO_ENGINE_SUBSTITUTION';
DELIMITER //
CREATE TRIGGER `info_AU` AFTER UPDATE ON `info` FOR EACH ROW BEGIN
	INSERT INTO log (IDplaca,receta,time,proceso,pasoProceso,estado,tiempoRestante,porcentaje) VALUES (NEW.IDplaca,NEW.receta,NOW(),NEW.proceso,NEW.pasoProceso,NEW.estado,NEW.tiempoRestante,NEW.porcentaje);
END//
DELIMITER ;
SET SQL_MODE=@OLDTMP_SQL_MODE;

-- Volcando estructura para disparador cervecero.placas_AI
SET @OLDTMP_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_ZERO_IN_DATE,NO_ZERO_DATE,NO_ENGINE_SUBSTITUTION';
DELIMITER //
CREATE TRIGGER placas_AI AFTER INSERT ON placas 
FOR EACH ROW BEGIN
	INSERT INTO menu (IDplaca) VALUES (NEW.id);
	INSERT INTO info (IDplaca) VALUES (NEW.id);
END//
DELIMITER ;
SET SQL_MODE=@OLDTMP_SQL_MODE;

/*!40101 SET SQL_MODE=IFNULL(@OLD_SQL_MODE, '') */;
/*!40014 SET FOREIGN_KEY_CHECKS=IF(@OLD_FOREIGN_KEY_CHECKS IS NULL, 1, @OLD_FOREIGN_KEY_CHECKS) */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
