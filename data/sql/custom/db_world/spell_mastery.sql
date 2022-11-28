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

 Date: 28/11/2022 10:34:53
*/

SET NAMES utf8mb4;
SET FOREIGN_KEY_CHECKS = 0;

-- ----------------------------
-- Table structure for spell_mastery
-- ----------------------------
DROP TABLE IF EXISTS `spell_mastery`;
CREATE TABLE `spell_mastery`  (
  `spellId` int(11) NOT NULL,
  `ratingPerOnePct` int(255) NULL DEFAULT NULL,
  PRIMARY KEY (`spellId`) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8mb4 COLLATE = utf8mb4_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of spell_mastery
-- ----------------------------
INSERT INTO `spell_mastery` VALUES (1000000, 46);

SET FOREIGN_KEY_CHECKS = 1;
