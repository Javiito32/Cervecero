-- --------------------------------------------------------
-- Host:                         127.0.0.1
-- Versión del servidor:         10.4.6-MariaDB - mariadb.org binary distribution
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

-- La exportación de datos fue deseleccionada.

-- Volcando estructura para tabla cervecero.firmwares
CREATE TABLE IF NOT EXISTS `firmwares` (
  `version` varchar(10) NOT NULL,
  `fechaSalida` timestamp NOT NULL DEFAULT current_timestamp() ON UPDATE current_timestamp(),
  PRIMARY KEY (`version`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

-- La exportación de datos fue deseleccionada.

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
  CONSTRAINT `FK_info_placas` FOREIGN KEY (`IDplaca`) REFERENCES `placas` (`IDplaca`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `FK_info_procesos` FOREIGN KEY (`proceso`) REFERENCES `procesos` (`id`) ON DELETE NO ACTION ON UPDATE CASCADE,
  CONSTRAINT `FK_info_recetas` FOREIGN KEY (`receta`) REFERENCES `recetas` (`ID`) ON DELETE NO ACTION ON UPDATE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=19 DEFAULT CHARSET=utf8mb4;

-- La exportación de datos fue deseleccionada.

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
  `porcentaje` tinyint(4) DEFAULT NULL,
  PRIMARY KEY (`ID`),
  KEY `estado` (`estado`),
  KEY `proceso` (`proceso`),
  KEY `placaID` (`IDplaca`) USING BTREE,
  KEY `FK_log_recetas` (`receta`),
  CONSTRAINT `FK_log_estados` FOREIGN KEY (`estado`) REFERENCES `estados` (`id`) ON DELETE NO ACTION ON UPDATE CASCADE,
  CONSTRAINT `FK_log_placas` FOREIGN KEY (`IDplaca`) REFERENCES `placas` (`IDplaca`) ON DELETE NO ACTION ON UPDATE CASCADE,
  CONSTRAINT `FK_log_procesos` FOREIGN KEY (`proceso`) REFERENCES `procesos` (`id`) ON DELETE NO ACTION ON UPDATE CASCADE,
  CONSTRAINT `FK_log_recetas` FOREIGN KEY (`receta`) REFERENCES `recetas` (`ID`) ON DELETE NO ACTION ON UPDATE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=488 DEFAULT CHARSET=utf8mb4;

-- La exportación de datos fue deseleccionada.

-- Volcando estructura para tabla cervecero.log_updates
CREATE TABLE IF NOT EXISTS `log_updates` (
  `IDplaca` int(11) NOT NULL,
  `fecha_update` timestamp NULL DEFAULT NULL,
  `oldVersion` varchar(10) DEFAULT NULL,
  `newVersion` varchar(10) DEFAULT NULL,
  `id` int(11) NOT NULL AUTO_INCREMENT,
  PRIMARY KEY (`id`),
  KEY `version` (`oldVersion`) USING BTREE,
  KEY `FK_log_updates_placas` (`IDplaca`),
  CONSTRAINT `FK_log_updates_firmwares` FOREIGN KEY (`oldVersion`) REFERENCES `firmwares` (`version`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `FK_log_updates_placas` FOREIGN KEY (`IDplaca`) REFERENCES `placas` (`IDplaca`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=40 DEFAULT CHARSET=utf8mb4;

-- La exportación de datos fue deseleccionada.

-- Volcando estructura para tabla cervecero.menu
CREATE TABLE IF NOT EXISTS `menu` (
  `IDplaca` int(11) NOT NULL DEFAULT 0,
  `menu` int(11) NOT NULL DEFAULT 0,
  `dato1` int(11) DEFAULT 0,
  `dato2` int(11) DEFAULT 0,
  `cancelar` int(11) NOT NULL DEFAULT 0,
  `updateNextBoot` int(11) DEFAULT 0,
  `continuar` int(11) DEFAULT 0,
  PRIMARY KEY (`IDplaca`) USING BTREE,
  CONSTRAINT `FK_menu_placas` FOREIGN KEY (`IDplaca`) REFERENCES `placas` (`IDplaca`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

-- La exportación de datos fue deseleccionada.

-- Volcando estructura para tabla cervecero.placas
CREATE TABLE IF NOT EXISTS `placas` (
  `IDplaca` int(11) NOT NULL AUTO_INCREMENT,
  `mac` varchar(17) CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_520_ci NOT NULL,
  `needUpdate` int(11) DEFAULT 0,
  `usuario` int(11) DEFAULT NULL,
  PRIMARY KEY (`IDplaca`) USING BTREE
) ENGINE=InnoDB AUTO_INCREMENT=19 DEFAULT CHARSET=utf8mb4;

-- La exportación de datos fue deseleccionada.

-- Volcando estructura para tabla cervecero.procesos
CREATE TABLE IF NOT EXISTS `procesos` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `proceso` varchar(17) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=6 DEFAULT CHARSET=utf8mb4;

-- La exportación de datos fue deseleccionada.

-- Volcando estructura para tabla cervecero.pruebas
CREATE TABLE IF NOT EXISTS `pruebas` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `json` longtext CHARACTER SET utf8mb4 COLLATE utf8mb4_bin NOT NULL CHECK (json_valid(`json`)),
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=utf8mb4;

-- La exportación de datos fue deseleccionada.

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
  `procesosOrden` varchar(50) NOT NULL DEFAULT '1:2:3:4',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=utf8mb4;

-- La exportación de datos fue deseleccionada.

-- Volcando estructura para tabla cervecero.updates
CREATE TABLE IF NOT EXISTS `updates` (
  `IDplaca` int(11) NOT NULL,
  `currentVersion` varchar(10) DEFAULT NULL,
  PRIMARY KEY (`IDplaca`),
  KEY `FK_updates_firmwares` (`currentVersion`) USING BTREE,
  CONSTRAINT `FK_updates_firmwares` FOREIGN KEY (`currentVersion`) REFERENCES `firmwares` (`version`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `FK_updates_placas` FOREIGN KEY (`IDplaca`) REFERENCES `placas` (`IDplaca`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

-- La exportación de datos fue deseleccionada.

-- Volcando estructura para tabla cervecero.users
CREATE TABLE IF NOT EXISTS `users` (
  `id` mediumint(8) unsigned NOT NULL AUTO_INCREMENT,
  `first_name` varchar(255) COLLATE utf8mb4_unicode_ci DEFAULT NULL,
  `last_name` varchar(255) COLLATE utf8mb4_unicode_ci DEFAULT NULL,
  `phone` varchar(100) COLLATE utf8mb4_unicode_ci DEFAULT NULL,
  `city` varchar(50) COLLATE utf8mb4_unicode_ci DEFAULT NULL,
  `zip` varchar(10) COLLATE utf8mb4_unicode_ci DEFAULT NULL,
  `updated_date` datetime DEFAULT NULL,
  `registered_date` datetime DEFAULT NULL,
  `removed_date` datetime DEFAULT NULL,
  `active` tinyint(1) DEFAULT NULL,
  `manager` int(11) DEFAULT NULL,
  `site` int(11) DEFAULT NULL,
  `image` int(11) DEFAULT NULL,
  `shift_start` time DEFAULT NULL,
  `shift_end` time DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=37 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;

-- La exportación de datos fue deseleccionada.

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
CREATE TRIGGER `placas_AI` AFTER INSERT ON `placas` FOR EACH ROW BEGIN
	INSERT INTO menu (IDplaca) VALUES (NEW.IDplaca);
	INSERT INTO info (IDplaca) VALUES (NEW.IDplaca);
	INSERT INTO updates (IDplaca) VALUES (NEW.IDplaca);
END//
DELIMITER ;
SET SQL_MODE=@OLDTMP_SQL_MODE;

-- Volcando estructura para disparador cervecero.updates_BU
SET @OLDTMP_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_ZERO_IN_DATE,NO_ZERO_DATE,NO_ENGINE_SUBSTITUTION';
DELIMITER //
CREATE TRIGGER `updates_BU` BEFORE UPDATE ON `updates` FOR EACH ROW BEGIN 
	INSERT INTO log_updates (IDplaca,fecha_update,oldVersion,newVersion) VALUES (NEW.IDplaca,NOW(),OLD.currentVersion,NEW.currentVersion);
END//
DELIMITER ;
SET SQL_MODE=@OLDTMP_SQL_MODE;

/*!40101 SET SQL_MODE=IFNULL(@OLD_SQL_MODE, '') */;
/*!40014 SET FOREIGN_KEY_CHECKS=IF(@OLD_FOREIGN_KEY_CHECKS IS NULL, 1, @OLD_FOREIGN_KEY_CHECKS) */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
