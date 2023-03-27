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

 Date: 25/03/2023 18:25:43
*/

SET NAMES utf8mb4;
SET FOREIGN_KEY_CHECKS = 0;

-- ----------------------------
-- Table structure for dungeon_mythic_bosses
-- ----------------------------
DROP TABLE IF EXISTS `dungeon_mythic_bosses`;
CREATE TABLE `dungeon_mythic_bosses`  (
  `mapId` int(0) NOT NULL,
  `order` int(0) NOT NULL,
  `bossId` int(0) NOT NULL
) ENGINE = InnoDB CHARACTER SET = utf8mb4 COLLATE = utf8mb4_0900_ai_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of dungeon_mythic_bosses
-- ----------------------------
INSERT INTO `dungeon_mythic_bosses` VALUES (36, 0, 644);
INSERT INTO `dungeon_mythic_bosses` VALUES (36, 1, 643);
INSERT INTO `dungeon_mythic_bosses` VALUES (36, 2, 1763);
INSERT INTO `dungeon_mythic_bosses` VALUES (36, 3, 646);
INSERT INTO `dungeon_mythic_bosses` VALUES (36, 6, 639);

SET FOREIGN_KEY_CHECKS = 1;
