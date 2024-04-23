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

 Date: 22/04/2024 11:26:05
*/

SET NAMES utf8mb4;
SET FOREIGN_KEY_CHECKS = 0;

-- ----------------------------
-- Table structure for character_combat_log_spells
-- ----------------------------
DROP TABLE IF EXISTS `character_combat_log_spells`;
CREATE TABLE `character_combat_log_spells`  (
  `combatLogId` int(11) NOT NULL,
  `spellId` bigint(20) NULL DEFAULT NULL,
  `amount` bigint(20) NULL DEFAULT NULL,
  `castedWhen` timestamp(6) NULL DEFAULT NULL
) ENGINE = InnoDB CHARACTER SET = utf8mb4 COLLATE = utf8mb4_general_ci ROW_FORMAT = Dynamic;

SET FOREIGN_KEY_CHECKS = 1;
