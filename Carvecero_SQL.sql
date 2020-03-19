-- --------------------------------------------------------
-- Host:                         127.0.0.1
-- Versión del servidor:         8.0.18 - MySQL Community Server - GPL
-- SO del servidor:              Win64
-- HeidiSQL Versión:             11.0.0.5919
-- --------------------------------------------------------

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!50503 SET NAMES utf8mb4 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;


-- Volcando estructura de base de datos para cervecero
CREATE DATABASE IF NOT EXISTS `cervecero` /*!40100 DEFAULT CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci */ /*!80016 DEFAULT ENCRYPTION='N' */;
USE `cervecero`;

-- Volcando estructura para tabla cervecero.estados
CREATE TABLE IF NOT EXISTS `estados` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `estado` varchar(50) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=4 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

-- Volcando datos para la tabla cervecero.estados: ~0 rows (aproximadamente)
/*!40000 ALTER TABLE `estados` DISABLE KEYS */;
REPLACE INTO `estados` (`id`, `estado`) VALUES
	(1, 'Iniciado'),
	(2, 'Finalizado'),
	(3, 'Fallo');
/*!40000 ALTER TABLE `estados` ENABLE KEYS */;

-- Volcando estructura para tabla cervecero.info
CREATE TABLE IF NOT EXISTS `info` (
  `ID` int(11) NOT NULL DEFAULT '1',
  `menu` int(11) NOT NULL DEFAULT '0',
  `fallo` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

-- Volcando datos para la tabla cervecero.info: ~1 rows (aproximadamente)
/*!40000 ALTER TABLE `info` DISABLE KEYS */;
REPLACE INTO `info` (`ID`, `menu`, `fallo`) VALUES
	(1, 0, 0);
/*!40000 ALTER TABLE `info` ENABLE KEYS */;

-- Volcando estructura para tabla cervecero.log
CREATE TABLE IF NOT EXISTS `log` (
  `ID` int(11) NOT NULL AUTO_INCREMENT,
  `time` timestamp NOT NULL,
  `proceso` int(11) DEFAULT NULL,
  `estado` int(11) DEFAULT NULL,
  PRIMARY KEY (`ID`),
  KEY `estado` (`estado`),
  KEY `proceso` (`proceso`),
  CONSTRAINT `FK_log_estados` FOREIGN KEY (`estado`) REFERENCES `estados` (`id`) ON UPDATE CASCADE,
  CONSTRAINT `FK_log_procesos` FOREIGN KEY (`proceso`) REFERENCES `procesos` (`id`) ON UPDATE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=31 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

-- Volcando datos para la tabla cervecero.log: ~8 rows (aproximadamente)
/*!40000 ALTER TABLE `log` DISABLE KEYS */;
REPLACE INTO `log` (`ID`, `time`, `proceso`, `estado`) VALUES
	(1, '2020-03-19 14:30:21', 1, 1),
	(2, '2020-03-19 14:30:33', 1, 3),
	(3, '2020-03-19 14:43:33', 1, 1),
	(4, '2020-03-19 14:44:35', 1, 2),
	(5, '2020-03-19 17:27:17', 3, 1),
	(6, '2020-03-19 17:30:10', 3, 1),
	(7, '2020-03-19 17:39:19', 3, 1),
	(8, '2020-03-19 17:39:22', 3, 2),
	(9, '2020-03-19 18:06:49', 3, 1),
	(10, '2020-03-19 18:07:51', 3, 3),
	(11, '2020-03-19 18:17:19', 3, 1),
	(12, '2020-03-19 18:18:45', 3, 2),
	(13, '2020-03-19 18:18:59', 3, 1),
	(14, '2020-03-19 18:19:32', 3, 3),
	(15, '2020-03-19 18:21:11', 3, 1),
	(16, '2020-03-19 18:22:29', 3, 3),
	(17, '2020-03-19 18:23:33', 1, 1),
	(18, '2020-03-19 18:24:34', 1, 2),
	(19, '2020-03-19 18:44:01', 1, 1),
	(20, '2020-03-19 18:45:02', 1, 2),
	(21, '2020-03-19 18:47:57', 1, 1),
	(22, '2020-03-19 18:48:58', 1, 2),
	(23, '2020-03-19 18:59:09', 1, 1),
	(24, '2020-03-19 19:00:10', 1, 2),
	(25, '2020-03-19 19:06:08', 2, 1),
	(26, '2020-03-19 19:06:21', 2, 3),
	(27, '2020-03-19 19:06:28', 3, 1),
	(28, '2020-03-19 19:06:47', 3, 3),
	(29, '2020-03-19 22:43:03', 1, 1),
	(30, '2020-03-19 22:43:30', 1, 3);
/*!40000 ALTER TABLE `log` ENABLE KEYS */;

-- Volcando estructura para tabla cervecero.procesos
CREATE TABLE IF NOT EXISTS `procesos` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `proceso` varchar(50) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=5 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

-- Volcando datos para la tabla cervecero.procesos: ~0 rows (aproximadamente)
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
  `tempMacer` float NOT NULL DEFAULT '0',
  `tiempoMacer` float NOT NULL DEFAULT '0',
  `tempCoc` float NOT NULL DEFAULT '0',
  `tiempoCoc` float NOT NULL DEFAULT '0',
  `tempFermen` float NOT NULL DEFAULT '0',
  `tiempoFermen` float NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

-- Volcando datos para la tabla cervecero.recetas: ~1 rows (aproximadamente)
/*!40000 ALTER TABLE `recetas` DISABLE KEYS */;
REPLACE INTO `recetas` (`ID`, `nombre`, `tempMacer`, `tiempoMacer`, `tempCoc`, `tiempoCoc`, `tempFermen`, `tiempoFermen`) VALUES
	(1, 'Pinta', 54, 1, 30, 50, 42, 1);
/*!40000 ALTER TABLE `recetas` ENABLE KEYS */;

/*!40101 SET SQL_MODE=IFNULL(@OLD_SQL_MODE, '') */;
/*!40014 SET FOREIGN_KEY_CHECKS=IF(@OLD_FOREIGN_KEY_CHECKS IS NULL, 1, @OLD_FOREIGN_KEY_CHECKS) */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
