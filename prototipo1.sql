# MySQL Navigator Xport
# Database: prototipo1
# root@localhost

# CREATE DATABASE prototipo1;
# USE prototipo1;

#
# Table structure for table 'jugadores'
#

# DROP TABLE IF EXISTS jugadores;
CREATE TABLE `jugadores` (
  `Nombre` varchar(20) NOT NULL,
  `Password` varchar(20) DEFAULT NULL,
  PRIMARY KEY (`Nombre`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

#
# Dumping data for table 'jugadores'
#

INSERT INTO jugadores VALUES ('','');
INSERT INTO jugadores VALUES ('armand','1234');
INSERT INTO jugadores VALUES ('bacelona','12454');
INSERT INTO jugadores VALUES ('binod','1234');
INSERT INTO jugadores VALUES ('binod1','1234');
INSERT INTO jugadores VALUES ('castelldefels','1234');
INSERT INTO jugadores VALUES ('fgfhcvhvb','561');
INSERT INTO jugadores VALUES ('joan','casamitjana');
INSERT INTO jugadores VALUES ('khanda','12541');
INSERT INTO jugadores VALUES ('lama','123');
INSERT INTO jugadores VALUES ('mikel','oesc');
INSERT INTO jugadores VALUES ('nada','1234');
INSERT INTO jugadores VALUES ('nadada','1234');
INSERT INTO jugadores VALUES ('nepal','1234');
INSERT INTO jugadores VALUES ('sdfsdfsdf','3262');
INSERT INTO jugadores VALUES ('upc','1234');
INSERT INTO jugadores VALUES ('upc1','1234');
INSERT INTO jugadores VALUES ('upc2','1234');
INSERT INTO jugadores VALUES ('upc4','1234');
INSERT INTO jugadores VALUES ('upc5','123456495');

#
# Table structure for table 'partidas'
#

# DROP TABLE IF EXISTS partidas;
CREATE TABLE `partidas` (
  `Idpartida` int(11) NOT NULL,
  `date_and_time` date DEFAULT NULL,
  `time` int(11) DEFAULT NULL,
  `winner` varchar(20) DEFAULT NULL,
  PRIMARY KEY (`Idpartida`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

#
# Dumping data for table 'partidas'
#

INSERT INTO partidas VALUES (1,'2016-10-15',30,'binod');
INSERT INTO partidas VALUES (2,'2016-10-15',30,'binod');
INSERT INTO partidas VALUES (3,'2017-10-15',30,'armand');

#
# Table structure for table 'relation'
#

# DROP TABLE IF EXISTS relation;
CREATE TABLE `relation` (
  `jugador` varchar(10) DEFAULT NULL,
  `partida` int(11) NOT NULL,
  KEY `jugador` (`jugador`),
  KEY `partida` (`partida`),
  CONSTRAINT `relation_ibfk_1` FOREIGN KEY (`jugador`) REFERENCES `jugadores` (`Nombre`),
  CONSTRAINT `relation_ibfk_2` FOREIGN KEY (`partida`) REFERENCES `partidas` (`Idpartida`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

#
# Dumping data for table 'relation'
#

INSERT INTO relation VALUES ('binod',1);
INSERT INTO relation VALUES ('binod',2);
INSERT INTO relation VALUES ('armand',3);

