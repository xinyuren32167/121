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

 Date: 22/04/2024 11:25:57
*/

SET NAMES utf8mb4;
SET FOREIGN_KEY_CHECKS = 0;

-- ----------------------------
-- Table structure for character_combat_log
-- ----------------------------
DROP TABLE IF EXISTS `character_combat_log`;
CREATE TABLE `character_combat_log`  (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `guid` bigint(20) NULL DEFAULT NULL,
  `mapId` int(11) NULL DEFAULT NULL,
  `specId` int(11) NULL DEFAULT NULL,
  `totalDamage` bigint(20) NULL DEFAULT NULL,
  `totalHealing` bigint(20) NULL DEFAULT NULL,
  `createdAt` datetime(0) NULL DEFAULT NULL,
  PRIMARY KEY (`id`) USING BTREE,
  INDEX `id`(`id`) USING BTREE
) ENGINE = InnoDB AUTO_INCREMENT = 20 CHARACTER SET = utf8mb4 COLLATE = utf8mb4_general_ci ROW_FORMAT = Dynamic;

SET FOREIGN_KEY_CHECKS = 1;
