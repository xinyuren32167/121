/*
 Navicat Premium Data Transfer

 Source Server         : Local
 Source Server Type    : MySQL
 Source Server Version : 50737
 Source Host           : localhost:3306
 Source Schema         : acore_world

 Target Server Type    : MySQL
 Target Server Version : 50737
 File Encoding         : 65001

 Date: 19/12/2022 19:45:28
*/

SET NAMES utf8mb4;
SET FOREIGN_KEY_CHECKS = 0;

-- ----------------------------
-- Table structure for runes
-- ----------------------------
DROP TABLE IF EXISTS `runes`;
CREATE TABLE `runes`  (
  `id` int(11) NOT NULL,
  `groupId` int(11) NULL DEFAULT NULL,
  `allowableClass` int(255) NULL DEFAULT NULL,
  `allowableRace` int(255) NULL DEFAULT NULL,
  `quality` tinyint(255) NULL DEFAULT NULL,
  `maxStack` tinyint(255) NULL DEFAULT NULL,
  `refundItemId` int(11) UNSIGNED NULL DEFAULT NULL,
  `refundDusts` int(255) NULL DEFAULT NULL,
  `keywords` varchar(255) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NULL DEFAULT NULL,
  PRIMARY KEY (`id`) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8mb4 COLLATE = utf8mb4_general_ci ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of runes
-- ----------------------------
INSERT INTO `runes` VALUES (100000, 0, -1, -1, 1, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (100001, 0, -1, -1, 2, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (100002, 0, -1, -1, 3, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (100003, 0, -1, -1, 4, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (100004, 0, -1, -1, 5, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (100005, 0, -1, -1, 6, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (100007, 1, -1, -1, 1, 2, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (100008, 1, -1, -1, 2, 2, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (100009, 1, -1, -1, 3, 2, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (100010, 1, -1, -1, 4, 2, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (100011, 1, -1, -1, 5, 2, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (100012, 1, -1, -1, 6, 2, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (100013, 2, -1, -1, 1, 2, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (100014, 2, -1, -1, 2, 2, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (100015, 2, -1, -1, 3, 2, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (100016, 2, -1, -1, 4, 2, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (100017, 2, -1, -1, 5, 2, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (100018, 2, -1, -1, 6, 2, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (100019, 3, -1, -1, 1, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (100020, 3, -1, -1, 2, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (100021, 3, -1, -1, 3, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (100022, 3, -1, -1, 4, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (100023, 3, -1, -1, 5, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (100024, 3, -1, -1, 6, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (100027, 4, -1, -1, 1, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (100028, 4, -1, -1, 2, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (100029, 4, -1, -1, 3, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (100030, 4, -1, -1, 4, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (100031, 4, -1, -1, 5, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (100032, 4, -1, -1, 6, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (100034, 5, -1, -1, 1, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (100035, 5, -1, -1, 2, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (100036, 5, -1, -1, 3, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (100037, 5, -1, -1, 4, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (100038, 5, -1, -1, 5, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (100039, 5, -1, -1, 6, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (100041, 6, -1, -1, 1, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (100042, 6, -1, -1, 2, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (100043, 6, -1, -1, 3, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (100044, 6, -1, -1, 4, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (100045, 6, -1, -1, 5, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (100046, 6, -1, -1, 6, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (100054, 7, -1, -1, 1, 2, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (100055, 7, -1, -1, 2, 2, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (100056, 7, -1, -1, 3, 2, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (100057, 7, -1, -1, 4, 2, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (100058, 7, -1, -1, 5, 2, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (100059, 7, -1, -1, 6, 2, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (100060, 8, -1, -1, 1, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (100061, 8, -1, -1, 2, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (100062, 8, -1, -1, 3, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (100063, 8, -1, -1, 4, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (100064, 8, -1, -1, 5, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (100065, 8, -1, -1, 6, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (100072, 9, -1, -1, 1, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (100073, 9, -1, -1, 2, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (100074, 9, -1, -1, 3, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (100075, 9, -1, -1, 4, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (100076, 9, -1, -1, 5, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (100077, 9, -1, -1, 6, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (100079, 10, -1, -1, 1, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (100080, 10, -1, -1, 2, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (100081, 10, -1, -1, 3, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (100082, 10, -1, -1, 4, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (100083, 10, -1, -1, 5, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (100084, 10, -1, -1, 6, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (100091, 11, -1, -1, 1, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (100092, 11, -1, -1, 2, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (100093, 11, -1, -1, 3, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (100094, 11, -1, -1, 4, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (100095, 11, -1, -1, 5, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (100096, 11, -1, -1, 6, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (100098, 12, -1, -1, 1, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (100099, 12, -1, -1, 2, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (100100, 12, -1, -1, 3, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (100101, 12, -1, -1, 4, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (100102, 12, -1, -1, 5, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (100103, 12, -1, -1, 6, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (100106, 13, -1, -1, 1, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (100107, 13, -1, -1, 2, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (100108, 13, -1, -1, 3, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (100109, 13, -1, -1, 4, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (100110, 13, -1, -1, 5, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (100111, 13, -1, -1, 6, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (100114, 14, -1, -1, 1, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (100115, 14, -1, -1, 2, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (100116, 14, -1, -1, 3, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (100117, 14, -1, -1, 4, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (100118, 14, -1, -1, 5, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (100119, 14, -1, -1, 6, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (100121, 15, -1, -1, 1, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (100122, 15, -1, -1, 2, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (100123, 15, -1, -1, 3, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (100124, 15, -1, -1, 4, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (100125, 15, -1, -1, 5, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (100126, 15, -1, -1, 6, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (100127, 16, -1, -1, 1, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (100128, 16, -1, -1, 2, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (100129, 16, -1, -1, 3, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (100130, 16, -1, -1, 4, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (100131, 16, -1, -1, 5, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (100132, 16, -1, -1, 6, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (100135, 17, -1, -1, 1, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (100136, 17, -1, -1, 2, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (100137, 17, -1, -1, 3, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (100138, 17, -1, -1, 4, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (100139, 17, -1, -1, 5, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (100140, 17, -1, -1, 6, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (100145, 18, -1, -1, 1, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (100146, 18, -1, -1, 2, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (100147, 18, -1, -1, 3, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (100148, 18, -1, -1, 4, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (100149, 18, -1, -1, 5, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (100150, 18, -1, -1, 6, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (100157, 19, -1, -1, 1, 3, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (100158, 19, -1, -1, 2, 3, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (100159, 19, -1, -1, 3, 3, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (100160, 19, -1, -1, 4, 3, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (100161, 19, -1, -1, 5, 3, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (100162, 19, -1, -1, 6, 3, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (100163, 20, -1, -1, 1, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (100164, 20, -1, -1, 2, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (100165, 20, -1, -1, 3, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (100166, 20, -1, -1, 4, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (100167, 20, -1, -1, 5, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (100168, 20, -1, -1, 6, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (100175, 21, -1, -1, 1, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (100176, 21, -1, -1, 2, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (100177, 21, -1, -1, 3, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (100178, 21, -1, -1, 4, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (100179, 21, -1, -1, 5, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (100180, 21, -1, -1, 6, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (100187, 22, -1, -1, 1, 2, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (100188, 22, -1, -1, 2, 2, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (100189, 22, -1, -1, 3, 2, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (100190, 22, -1, -1, 4, 2, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (100191, 22, -1, -1, 5, 2, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (100192, 22, -1, -1, 6, 2, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (100193, 23, -1, -1, 1, 2, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (100194, 23, -1, -1, 2, 2, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (100195, 23, -1, -1, 3, 2, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (100196, 23, -1, -1, 4, 2, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (100197, 23, -1, -1, 5, 2, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (100198, 23, -1, -1, 6, 2, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (100199, 24, -1, -1, 1, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (100200, 24, -1, -1, 2, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (100201, 24, -1, -1, 3, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (100202, 24, -1, -1, 4, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (100203, 24, -1, -1, 5, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (100204, 24, -1, -1, 6, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (100211, 25, -1, -1, 1, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (100212, 25, -1, -1, 2, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (100213, 25, -1, -1, 3, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (100214, 25, -1, -1, 4, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (100215, 25, -1, -1, 5, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (100216, 25, -1, -1, 6, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (100218, 26, -1, -1, 1, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (100219, 26, -1, -1, 2, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (100220, 26, -1, -1, 3, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (100221, 26, -1, -1, 4, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (100222, 26, -1, -1, 5, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (100223, 26, -1, -1, 6, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (100225, 27, -1, -1, 1, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (100226, 27, -1, -1, 2, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (100227, 27, -1, -1, 3, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (100228, 27, -1, -1, 4, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (100229, 27, -1, -1, 5, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (100230, 27, -1, -1, 6, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (100232, 28, -1, -1, 1, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (100233, 28, -1, -1, 2, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (100234, 28, -1, -1, 3, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (100235, 28, -1, -1, 4, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (100236, 28, -1, -1, 5, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (100237, 28, -1, -1, 6, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (100240, 29, -1, -1, 1, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (100241, 29, -1, -1, 2, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (100242, 29, -1, -1, 3, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (100243, 29, -1, -1, 4, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (100244, 29, -1, -1, 5, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (100245, 29, -1, -1, 6, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (100253, 30, -1, -1, 1, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (100254, 30, -1, -1, 2, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (100255, 30, -1, -1, 3, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (100256, 30, -1, -1, 4, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (100257, 30, -1, -1, 5, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (100258, 30, -1, -1, 6, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (100271, 31, -1, -1, 1, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (100272, 31, -1, -1, 2, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (100273, 31, -1, -1, 3, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (100274, 31, -1, -1, 4, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (100275, 31, -1, -1, 5, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (100276, 31, -1, -1, 6, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (200008, 32, 1, -1, 1, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (200009, 32, 1, -1, 2, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (200010, 32, 1, -1, 3, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (200011, 32, 1, -1, 4, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (200012, 32, 1, -1, 5, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (200013, 32, 1, -1, 6, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (200014, 33, 1, -1, 1, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (200015, 33, 1, -1, 2, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (200016, 33, 1, -1, 3, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (200017, 33, 1, -1, 4, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (200018, 33, 1, -1, 5, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (200019, 33, 1, -1, 6, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (200026, 34, 1, -1, 1, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (200027, 34, 1, -1, 2, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (200028, 34, 1, -1, 3, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (200029, 34, 1, -1, 4, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (200030, 34, 1, -1, 5, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (200031, 34, 1, -1, 6, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (200038, 35, 1, -1, 1, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (200039, 35, 1, -1, 2, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (200040, 35, 1, -1, 3, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (200041, 35, 1, -1, 4, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (200042, 35, 1, -1, 5, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (200043, 35, 1, -1, 6, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (200045, 36, 1, -1, 1, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (200046, 36, 1, -1, 2, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (200047, 36, 1, -1, 3, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (200048, 36, 1, -1, 4, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (200049, 36, 1, -1, 5, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (200050, 36, 1, -1, 6, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (200057, 37, 1, -1, 1, 2, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (200058, 37, 1, -1, 2, 2, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (200059, 37, 1, -1, 3, 2, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (200060, 37, 1, -1, 4, 2, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (200061, 37, 1, -1, 5, 2, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (200062, 37, 1, -1, 6, 2, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (200063, 38, 1, -1, 1, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (200064, 38, 1, -1, 2, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (200065, 38, 1, -1, 3, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (200066, 38, 1, -1, 4, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (200067, 38, 1, -1, 5, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (200068, 38, 1, -1, 6, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (200075, 39, 1, -1, 1, 3, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (200076, 39, 1, -1, 2, 3, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (200077, 39, 1, -1, 3, 3, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (200078, 39, 1, -1, 4, 3, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (200079, 39, 1, -1, 5, 3, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (200080, 39, 1, -1, 6, 3, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (200081, 40, 1, -1, 1, 2, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (200082, 40, 1, -1, 2, 2, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (200083, 40, 1, -1, 3, 2, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (200084, 40, 1, -1, 4, 2, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (200085, 40, 1, -1, 5, 2, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (200086, 40, 1, -1, 6, 2, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (200087, 41, 1, -1, 1, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (200088, 41, 1, -1, 2, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (200089, 41, 1, -1, 3, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (200090, 41, 1, -1, 4, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (200091, 41, 1, -1, 5, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (200092, 41, 1, -1, 6, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (200094, 42, 1, -1, 1, 3, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (200095, 42, 1, -1, 2, 3, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (200096, 42, 1, -1, 3, 3, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (200097, 42, 1, -1, 4, 3, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (200098, 42, 1, -1, 5, 3, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (200099, 42, 1, -1, 6, 3, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (200100, 43, 1, -1, 1, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (200101, 43, 1, -1, 2, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (200102, 43, 1, -1, 3, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (200103, 43, 1, -1, 4, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (200104, 43, 1, -1, 5, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (200105, 43, 1, -1, 6, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (200108, 44, 1, -1, 1, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (200109, 44, 1, -1, 2, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (200110, 44, 1, -1, 3, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (200111, 44, 1, -1, 4, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (200112, 44, 1, -1, 5, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (200113, 44, 1, -1, 6, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (200115, 45, 1, -1, 1, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (200116, 45, 1, -1, 2, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (200117, 45, 1, -1, 3, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (200118, 45, 1, -1, 4, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (200119, 45, 1, -1, 5, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (200120, 45, 1, -1, 6, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (200127, 46, 1, -1, 1, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (200128, 46, 1, -1, 2, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (200129, 46, 1, -1, 3, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (200130, 46, 1, -1, 4, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (200131, 46, 1, -1, 5, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (200132, 46, 1, -1, 6, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (200133, 47, 1, -1, 1, 2, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (200134, 47, 1, -1, 2, 2, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (200135, 47, 1, -1, 3, 2, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (200136, 47, 1, -1, 4, 2, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (200137, 47, 1, -1, 5, 2, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (200138, 47, 1, -1, 6, 2, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (200139, 48, 1, -1, 1, 2, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (200140, 48, 1, -1, 2, 2, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (200141, 48, 1, -1, 3, 2, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (200142, 48, 1, -1, 4, 2, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (200143, 48, 1, -1, 5, 2, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (200144, 48, 1, -1, 6, 2, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (200145, 49, 1, -1, 1, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (200146, 49, 1, -1, 2, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (200147, 49, 1, -1, 3, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (200148, 49, 1, -1, 4, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (200149, 49, 1, -1, 5, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (200150, 49, 1, -1, 6, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (200157, 50, 1, -1, 1, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (200158, 50, 1, -1, 2, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (200159, 50, 1, -1, 3, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (200160, 50, 1, -1, 4, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (200161, 50, 1, -1, 5, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (200162, 50, 1, -1, 6, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (200169, 51, 1, -1, 1, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (200170, 51, 1, -1, 2, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (200171, 51, 1, -1, 3, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (200172, 51, 1, -1, 4, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (200173, 51, 1, -1, 5, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (200174, 51, 1, -1, 6, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (200175, 52, 1, -1, 1, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (200176, 52, 1, -1, 2, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (200177, 52, 1, -1, 3, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (200178, 52, 1, -1, 4, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (200179, 52, 1, -1, 5, 1, 0, 0, 'Temporary');
INSERT INTO `runes` VALUES (200180, 52, 1, -1, 6, 1, 0, 0, 'Temporary');

SET FOREIGN_KEY_CHECKS = 1;
