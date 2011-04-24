-- phpMyAdmin SQL Dump
-- version 3.3.10
-- http://www.phpmyadmin.net
--
-- Host: localhost
-- Erstellungszeit: 24. April 2011 um 17:18
-- Server Version: 5.5.11
-- PHP-Version: 5.3.6

SET SQL_MODE="NO_AUTO_VALUE_ON_ZERO";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;

--
-- Datenbank: `chars`
--

-- --------------------------------------------------------

--
-- Tabellenstruktur für Tabelle `characters`
--

CREATE TABLE IF NOT EXISTS `characters` (
  `id` int(10) NOT NULL AUTO_INCREMENT,
  `name` varchar(255) NOT NULL,
  `faction` int(10) NOT NULL,
  `player` int(10) NOT NULL,
  `exp` int(10) NOT NULL,
  `level` int(3) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8 AUTO_INCREMENT=2 ;

--
-- Daten für Tabelle `characters`
--

INSERT INTO `characters` (`id`, `name`, `faction`, `player`, `exp`, `level`) VALUES
(1, 'Heinrich Zimmermann', 0, 1, 132, 3);

-- --------------------------------------------------------

--
-- Tabellenstruktur für Tabelle `factions`
--

CREATE TABLE IF NOT EXISTS `factions` (
  `id` int(10) NOT NULL AUTO_INCREMENT,
  `faction` varchar(255) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8 AUTO_INCREMENT=6 ;

--
-- Daten für Tabelle `factions`
--

INSERT INTO `factions` (`id`, `faction`) VALUES
(1, 'Humans of Stormwind'),
(2, 'Orcs of Orgrimmar'),
(3, 'Dwarves of Ironforge'),
(4, 'Nightelves'),
(5, 'Pirates of Booty Bay');

-- --------------------------------------------------------

--
-- Tabellenstruktur für Tabelle `options`
--

CREATE TABLE IF NOT EXISTS `options` (
  `name` varchar(255) NOT NULL,
  `value` varchar(255) NOT NULL,
  PRIMARY KEY (`name`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Daten für Tabelle `options`
--

INSERT INTO `options` (`name`, `value`) VALUES
('motd', 'Hello World I am sooo awesome!');

-- --------------------------------------------------------

--
-- Tabellenstruktur für Tabelle `users`
--

CREATE TABLE IF NOT EXISTS `users` (
  `id` int(10) NOT NULL AUTO_INCREMENT,
  `name` varchar(255) NOT NULL,
  `password` varchar(255) NOT NULL,
  `email` varchar(255) NOT NULL,
  `lastlogin` int(16) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8 AUTO_INCREMENT=2 ;

--
-- Daten für Tabelle `users`
--

INSERT INTO `users` (`id`, `name`, `password`, `email`, `lastlogin`) VALUES
(1, 'opatut', 'a4b82191bae72fdc59742b365419a2e1b5bc75735b8e56fcdf122c033bacca1d', 'opatutlol@aol.com', 0);
