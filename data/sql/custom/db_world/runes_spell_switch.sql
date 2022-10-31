/*
 Navicat Premium Data Transfer

 Source Server         : 127.0.0.1
 Source Server Type    : MySQL
 Source Server Version : 50731
 Source Host           : localhost:3306
 Source Schema         : world

 Target Server Type    : MySQL
 Target Server Version : 50731
 File Encoding         : 65001

 Date: 31/10/2022 17:08:34
*/

SET NAMES utf8mb4;
SET FOREIGN_KEY_CHECKS = 0;

-- ----------------------------
-- Table structure for runes_spell_switch
-- ----------------------------
DROP TABLE IF EXISTS `runes_spell_switch`;
CREATE TABLE `runes_spell_switch`  (
  `rune_spell_id` int(11) NOT NULL,
  `old_spell_id` int(11) NULL DEFAULT NULL,
  `new_spell_id` int(11) NULL DEFAULT NULL,
  PRIMARY KEY (`rune_spell_id`) USING BTREE
) ENGINE = InnoDB CHARACTER SET = latin1 COLLATE = latin1_swedish_ci ROW_FORMAT = Dynamic;

SET FOREIGN_KEY_CHECKS = 1;
