const Utils = require('./utils/Utils');
const Squad = require('./Squad');

class Army {
  id = -1;
  name = '';
  attackStrategy = ''; // random, weakest, strongest
  numberOfSquads = 2; // >= 2
  numberOfUnitsPerSquad = 5; // >=5, <= 10
  godOfWar = null;

  squads = {};
  defeatedSquads = {};

  constructor(id, name, attackStrategy, numberOfSquads, numberOfUnitsPerSquad, godOfWar) {
    this.id = id;
    this.name = name;
    this.attackStrategy = attackStrategy;
    this.numberOfSquads = numberOfSquads;
    this.numberOfUnitsPerSquad = numberOfUnitsPerSquad;
    this.godOfWar = godOfWar;

    this.createSquads(numberOfSquads);
  }

  getId() {
    return this.id;
  }

  getName() {
    return this.name;
  }

  getAttackStrategy() {
    return this.attackStrategy;
  }

  defeatSquad(squadId) {
    this.defeatedSquads[squadId] = this.squads[squadId];
    delete this.squads[squadId];
  }

  createSquads(numberOfSquads) {
    let i = 0;
    while (numberOfSquads--) {
      this.squads[i] = new Squad(i, this.numberOfUnitsPerSquad, this.id);
      this.squads[i].setOnRechargeCallback(this.onSquadRecharge.bind(this));
      ++i;
    }
  }

  static squadKeysSortingPredicate(k0, k1) {
    let s0 = this.squads[k0];
    let s1 = this.squads[k1];
    let healthDiff = Math.sign(s0.getTotalHealth() - s1.getTotalHealth());
    if (healthDiff !== 0) {
      return healthDiff;
    }
    let experiencePerUnitDiff = Math.sign(s0.getExperiencePerUnit() - s1.getExperiencePerUnit());
    if (experiencePerUnitDiff !== 0) {
      return experiencePerUnitDiff;
    }
    let numberOfUnitsDiff = Math.sign(s0.getNumberOfUnits() - s1.getNumberOfUnits());
    if (numberOfUnitsDiff !== 0) {
      return numberOfUnitsDiff;
    }
    let totalDamageDiff = Math.sign(s0.getTotalDamage() - s1.getTotalDamage());
    if (totalDamageDiff !== 0) {
      return totalDamageDiff;
    }
    return 0;
  }

  static squadsSortingPredicate(s0, s1) {
    let healthDiff = Math.sign(s0.getTotalHealth() - s1.getTotalHealth());
    if (healthDiff !== 0) {
      return healthDiff;
    }
    let experiencePerUnitDiff = Math.sign(s0.getExperiencePerUnit() - s1.getExperiencePerUnit());
    if (experiencePerUnitDiff !== 0) {
      return experiencePerUnitDiff;
    }
    let numberOfUnitsDiff = Math.sign(s0.getNumberOfUnits() - s1.getNumberOfUnits());
    if (numberOfUnitsDiff !== 0) {
      return numberOfUnitsDiff;
    }
    let totalDamageDiff = Math.sign(s0.getTotalDamage() - s1.getTotalDamage());
    if (totalDamageDiff !== 0) {
      return totalDamageDiff;
    }
    return 0;
  }

  sortSquadsByStrength() {
    return Object.values(this.squads).sort(Army.squadsSortingPredicate);
  }

  getRandomSquad() {
    return Utils.getRandomArrayValue(Object.values(this.squads));
  }

  getWeakestSquad() {
    let sortedSquads = this.sortSquadsByStrength();
    if (sortedSquads.length > 0) {
      return sortedSquads[0];
    }
    return null;
  }

  getStrongestSquad() {
    let sortedSquads = this.sortSquadsByStrength();
    let length = sortedSquads.length;
    if (length > 0) {
      return sortedSquads[length - 1];
    }
    return null;
  }

  recharge() {
    Object.values(this.squads).forEach(squad => squad.recharge());
  }

  onSquadRecharge(squad) { 
    if (!this.godOfWar.isWarEnded()) {
      this.godOfWar.attack(squad, this);
    }
  }

  isActive() {
    return Object.keys(this.squads).length > 0;
  }
}

module.exports = Army;
