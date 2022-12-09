/*
 Navicat Premium Data Transfer

 Source Server         : 127.0.0.1
 Source Server Type    : MySQL
 Source Server Version : 50731
 Source Host           : localhost:3306
 Source Schema         : acore_characters

 Target Server Type    : MySQL
 Target Server Version : 50731
 File Encoding         : 65001

 Date: 06/12/2022 10:49:48
*/

SET NAMES utf8mb4;
SET FOREIGN_KEY_CHECKS = 0;

-- ----------------------------
-- Table structure for character_reputation
-- ----------------------------
DROP TABLE IF EXISTS `character_reputation`;
CREATE TABLE `character_reputation`  (
  `accountId` int(10) UNSIGNED NOT NULL DEFAULT 0 COMMENT 'Global Unique Identifier',
  `faction` smallint(5) UNSIGNED NOT NULL DEFAULT 0,
  `standing` int(11) NOT NULL DEFAULT 0,
  `flags` smallint(5) UNSIGNED NOT NULL DEFAULT 0,
  PRIMARY KEY (`accountId`, `faction`) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8mb4 COLLATE = utf8mb4_general_ci COMMENT = 'Player System' ROW_FORMAT = Dynamic;

SET FOREIGN_KEY_CHECKS = 1;
