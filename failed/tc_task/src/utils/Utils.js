class Utils {
  static getRandomArrayIndex(arr) {
    if (arr.length === 0) {
      return -1;
    }
    return Math.floor(arr.length * Math.random());
  }

  static getRandomArrayValue(arr) {
    if (arr.length === 0) {
      return undefined;
    }
    return arr[Utils.getRandomArrayIndex(arr)];
  }

  static getRandomFromSegment(left, right) {
    return left + Math.floor((right - left + 1) * Math.random());
  }
}

module.exports = Utils;
