-- phpMyAdmin SQL Dump
-- version 4.7.0
-- https://www.phpmyadmin.net/
--
-- Host: 127.0.0.1
-- Generation Time: Jul 27, 2017 at 02:15 AM
-- Server version: 10.1.25-MariaDB
-- PHP Version: 7.1.7

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
SET AUTOCOMMIT = 0;
START TRANSACTION;
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- Database: `esp`
--

-- --------------------------------------------------------

--
-- Table structure for table `esptable`
--

CREATE TABLE `esptable` (
  `Time` timestamp(6) NOT NULL DEFAULT CURRENT_TIMESTAMP(6) ON UPDATE CURRENT_TIMESTAMP(6),
  `id` int(11) NOT NULL,
  `SENSOR` int(11) NOT NULL,
  `SENSOR1` int(11) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1 COMMENT='Sensors';

--
-- Dumping data for table `esptable`
--

INSERT INTO `esptable` (`Time`, `id`, `SENSOR`, `SENSOR1`) VALUES
('2017-07-27 00:14:55.675090', 1, 521, 57);

--
-- Triggers `esptable`
--
DELIMITER $$
CREATE TRIGGER `Update` BEFORE UPDATE ON `esptable` FOR EACH ROW BEGIN
        IF OLD.SENSOR != NEW.SENSOR
        THEN
                INSERT INTO update12
                    (
                        SENSOR
                    )
                    VALUES
                    (
                        NEW.SENSOR
                    );
        END IF;
 
    END
$$
DELIMITER ;

--
-- Indexes for dumped tables
--

--
-- Indexes for table `esptable`
--
ALTER TABLE `esptable`
  ADD PRIMARY KEY (`id`);

--
-- AUTO_INCREMENT for dumped tables
--

--
-- AUTO_INCREMENT for table `esptable`
--
ALTER TABLE `esptable`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=2;COMMIT;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
