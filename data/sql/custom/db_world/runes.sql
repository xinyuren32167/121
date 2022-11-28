/*
 Navicat Premium Data Transfer

 Source Server         : 127.0.0.1
 Source Server Type    : MySQL
 Source Server Version : 50731
 Source Host           : localhost:3306
 Source Schema         : acore_world

 Target Server Type    : MySQL
 Target Server Version : 50731
 File Encoding         : 65001

 Date: 28/11/2022 15:37:42
*/

SET NAMES utf8mb4;
SET FOREIGN_KEY_CHECKS = 0;

-- ----------------------------
-- Table structure for runes
-- ----------------------------
DROP TABLE IF EXISTS `runes`;
CREATE TABLE `runes`  (
  `id` int(11) NOT NULL,
  `groupId` int(11) NULL DEFAULT NULL,
  `allowableClass` int(255) NULL DEFAULT NULL,
  `allowableRace` int(255) NULL DEFAULT NULL,
  `quality` tinyint(255) NULL DEFAULT NULL,
  `maxStack` tinyint(255) NULL DEFAULT NULL,
  `refundItemId` int(11) UNSIGNED NULL DEFAULT NULL,
  `refundDusts` int(255) NULL DEFAULT NULL,
  `keywords` varchar(255) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NULL DEFAULT NULL,
  PRIMARY KEY (`id`) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8mb4 COLLATE = utf8mb4_general_ci ROW_FORMAT = Dynamic;

SET FOREIGN_KEY_CHECKS = 1;
