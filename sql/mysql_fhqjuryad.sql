-- phpMyAdmin SQL Dump
-- version 4.6.6deb5
-- https://www.phpmyadmin.net/

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
SET time_zone = "+00:00";

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

CREATE TABLE `flags` (
  `id` int UNSIGNED NOT NULL,
  `serviceid` int UNSIGNED DEFAULT NULL,
  `flag_id` varchar(50) DEFAULT NULL,
  `flag` varchar(36) DEFAULT NULL,
  `teamid` varchar(300) DEFAULT NULL,
  `date_start` bigint DEFAULT NULL,
  `date_end` bigint DEFAULT NULL,
  `team_stole` int UNSIGNED DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

ALTER TABLE `flags` ADD PRIMARY KEY (`id`);
ALTER TABLE `flags` ADD INDEX( `teamid`, `team_stole`);

CREATE TABLE `flags_live` (
  `id` int UNSIGNED NOT NULL,
  `serviceid` int UNSIGNED DEFAULT NULL,
  `flag_id` varchar(50) DEFAULT NULL,
  `flag` varchar(36) DEFAULT NULL,
  `teamid` int UNSIGNED DEFAULT NULL,
  `date_start` bigint DEFAULT NULL,
  `date_end` bigint DEFAULT NULL,
  `team_stole` int UNSIGNED DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

ALTER TABLE `flags_live`
  ADD PRIMARY KEY (`id`);

ALTER TABLE `flags`
  MODIFY `id` int(10) UNSIGNED NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=1;

ALTER TABLE `flags_live`
  MODIFY `id` int(10) UNSIGNED NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=1;
