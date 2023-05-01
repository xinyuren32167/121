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

 Date: 25/03/2023 18:25:33
*/

SET NAMES utf8mb4;
SET FOREIGN_KEY_CHECKS = 0;

-- ----------------------------
-- Table structure for dungeon_mythic_affixes
-- ----------------------------
DROP TABLE IF EXISTS `dungeon_mythic_affixes`;
CREATE TABLE `dungeon_mythic_affixes`  (
  `spellId` int(0) NOT NULL,
  `level` varchar(255) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NULL DEFAULT NULL,
  `week` varchar(255) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NOT NULL,
  PRIMARY KEY (`spellId`, `week`) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8mb4 COLLATE = utf8mb4_0900_ai_ci ROW_FORMAT = Dynamic;

SET FOREIGN_KEY_CHECKS = 1;
