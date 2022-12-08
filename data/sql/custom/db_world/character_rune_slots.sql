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

 Date: 08/12/2022 16:36:25
*/

SET NAMES utf8mb4;
SET FOREIGN_KEY_CHECKS = 0;

-- ----------------------------
-- Table structure for character_rune_slots
-- ----------------------------
DROP TABLE IF EXISTS `character_rune_slots`;
CREATE TABLE `character_rune_slots`  (
  `id` int(11) NOT NULL,
  `runeId` int(11) NULL DEFAULT NULL,
  `runeSpellId` int(11) NULL DEFAULT NULL,
  `order` int(255) NULL DEFAULT NULL
) ENGINE = InnoDB CHARACTER SET = utf8mb4 COLLATE = utf8mb4_general_ci ROW_FORMAT = Dynamic;

SET FOREIGN_KEY_CHECKS = 1;
