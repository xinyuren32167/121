/*
 Navicat Premium Data Transfer

 Source Server         : Valanior
 Source Server Type    : MySQL
 Source Server Version : 80032
 Source Host           : 151.80.58.21:3306
 Source Schema         : acore_world

 Target Server Type    : MySQL
 Target Server Version : 80032
 File Encoding         : 65001

 Date: 25/03/2023 18:26:02
*/

SET NAMES utf8mb4;
SET FOREIGN_KEY_CHECKS = 0;

-- ----------------------------
-- Table structure for runes_spell_switch
-- ----------------------------
DROP TABLE IF EXISTS `runes_spell_switch`;
CREATE TABLE `runes_spell_switch`  (
  `rune_spell_id` int(0) NOT NULL,
  `old_spell_id` int(0) NULL DEFAULT NULL,
  `new_spell_id` int(0) NULL DEFAULT NULL,
  PRIMARY KEY (`rune_spell_id`) USING BTREE
) ENGINE = InnoDB CHARACTER SET = latin1 COLLATE = latin1_swedish_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of runes_spell_switch
-- ----------------------------
INSERT INTO `runes_spell_switch` VALUES (300918, 42940, 300916);
INSERT INTO `runes_spell_switch` VALUES (300919, 42940, 300916);
INSERT INTO `runes_spell_switch` VALUES (300920, 42940, 300916);
INSERT INTO `runes_spell_switch` VALUES (300921, 42940, 300916);
INSERT INTO `runes_spell_switch` VALUES (300922, 42940, 300916);
INSERT INTO `runes_spell_switch` VALUES (300923, 42940, 300916);
INSERT INTO `runes_spell_switch` VALUES (300926, 42846, 300924);
INSERT INTO `runes_spell_switch` VALUES (300927, 42846, 300924);
INSERT INTO `runes_spell_switch` VALUES (300928, 42846, 300924);
INSERT INTO `runes_spell_switch` VALUES (300929, 42846, 300924);
INSERT INTO `runes_spell_switch` VALUES (300930, 42846, 300924);
INSERT INTO `runes_spell_switch` VALUES (300931, 42846, 300924);
INSERT INTO `runes_spell_switch` VALUES (500604, 80155, 80189);
INSERT INTO `runes_spell_switch` VALUES (500605, 80155, 80189);
INSERT INTO `runes_spell_switch` VALUES (500606, 80155, 80189);
INSERT INTO `runes_spell_switch` VALUES (500607, 80155, 80189);
INSERT INTO `runes_spell_switch` VALUES (500608, 80155, 80189);
INSERT INTO `runes_spell_switch` VALUES (500609, 80155, 80189);

SET FOREIGN_KEY_CHECKS = 1;
