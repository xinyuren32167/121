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

 Date: 13/03/2023 18:41:52
*/

SET NAMES utf8mb4;
SET FOREIGN_KEY_CHECKS = 0;

-- ----------------------------
-- Table structure for character_mythic_key
-- ----------------------------
DROP TABLE IF EXISTS `character_mythic_key`;
CREATE TABLE `character_mythic_key`  (
  `guid` int(11) NOT NULL,
  `mapId` int(11) NULL DEFAULT NULL,
  `level` int(255) NULL DEFAULT NULL,
  `createdAt` datetime(0) NULL DEFAULT NULL,
  PRIMARY KEY (`guid`) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8mb4 COLLATE = utf8mb4_general_ci ROW_FORMAT = Dynamic;

SET FOREIGN_KEY_CHECKS = 1;
