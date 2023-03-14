/*
 Navicat Premium Data Transfer

 Source Server         : Valanior
 Source Server Type    : MySQL
 Source Server Version : 80032
 Source Host           : 151.80.58.21:3306
 Source Schema         : acore_characters

 Target Server Type    : MySQL
 Target Server Version : 80032
 File Encoding         : 65001

 Date: 11/03/2023 17:14:06
*/

SET NAMES utf8mb4;
SET FOREIGN_KEY_CHECKS = 0;

-- ----------------------------
-- Table structure for character_mythic_completed
-- ----------------------------
DROP TABLE IF EXISTS `character_mythic_completed`;
CREATE TABLE `character_mythic_completed`  (
  `id` int(0) NOT NULL AUTO_INCREMENT,
  `guid` int(0) NOT NULL,
  `mapId` int(0) NULL DEFAULT NULL,
  `level` varchar(255) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NULL DEFAULT NULL,
  `timer` tinyint(0) NULL DEFAULT NULL,
  `createdAt` datetime(0) NULL DEFAULT NULL,
  `season` int(0) NULL DEFAULT 1,
  PRIMARY KEY (`id`) USING BTREE,
  INDEX `createdAt`(`createdAt`) USING BTREE
) ENGINE = InnoDB AUTO_INCREMENT = 6 CHARACTER SET = utf8mb4 COLLATE = utf8mb4_0900_ai_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of character_mythic_completed
-- ----------------------------
INSERT INTO `character_mythic_completed` VALUES (1, 1, 1, '1', 1, '2023-03-14 21:55:59', 1);
INSERT INTO `character_mythic_completed` VALUES (2, 1, 1, '2', 1, '2023-03-08 21:55:59', 1);
INSERT INTO `character_mythic_completed` VALUES (3, 2, 1, '1', 1, '2023-03-08 21:55:59', 1);
INSERT INTO `character_mythic_completed` VALUES (4, 2, 1, '2', 1, '2023-03-08 21:55:59', 1);
INSERT INTO `character_mythic_completed` VALUES (5, 2, 2, '2', 1, '2023-04-01 14:32:53', 1);

SET FOREIGN_KEY_CHECKS = 1;
