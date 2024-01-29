<template>
  <main class="grep">
    <div class="grep__folder-selection folder-selection">
      <h3 class="folder-selection__title">Welcome to GREP!</h3>
      <q-form class="folder-selection__form" @submit.prevent="onSubmit">
        <q-input v-model="folderLocation" v-bind="fieldConfig.folder">
        </q-input>
        <q-input
          v-model="regex"
          v-bind="fieldConfig.regex"
          v-if="isRegexMode"
        ></q-input>
        <q-input
          v-model="searchedChar"
          v-bind="fieldConfig.searchedChar"
          v-else
        ></q-input>

        <div class="folder-selection__toggle">
          <q-toggle v-model="isRegexMode" dark size="md">regex mode</q-toggle>
        </div>

        <q-btn v-bind="buttonConfig" :loading="isLoading" />
      </q-form>
    </div>
    <div class="grep__output output">
      <div class="output__charts">
        <q-knob
          v-model="matches.length"
          size="120px"
          v-if="matches.length"
          show-value
          track-color="secondary"
        />
        <q-knob
          v-model="discovered.length"
          size="120px"
          v-if="discovered.length"
          show-value
          track-color="secondary"
        />
      </div>

      <h3 v-if="matches.length">lines Matched:</h3>

      <q-breadcrumbs v-for="{ path, line, index } in matches">
        <q-breadcrumbs-el :label="folder" v-for="folder in path.split('/')" />
        <q-breadcrumbs-el :label="`line ${line}`" />
        <q-breadcrumbs-el :label="`index ${index}`" />
        <template v-slot:separator>
          <q-icon size="1.2em" color="primary" name="note" />
        </template>
      </q-breadcrumbs>

      <h3 v-if="discovered.length">files found:</h3>

      <q-breadcrumbs v-for="{ path, time } in discovered">
        <q-breadcrumbs-el :label="folder" v-for="folder in path.split('/')" />
        <q-breadcrumbs-el :label="`time spend ${time}ms`" />
        <template v-slot:separator>
          <q-icon size="1.2em" color="primary" name="note" />
        </template>
      </q-breadcrumbs>
    </div>
  </main>
</template>

<script setup>
import { ref } from "vue";

const folderLocation = ref("");
const regex = ref("");
const searchedChar = ref("");
const isRegexMode = ref(false);

const isLoading = ref(false);

const requiredRule = (name) => (value) => !!value || "folder is required!";

const defaultFieldConfig = {
  filled: true,
  bgColor: "secondary",
  square: true,
  rules: [() => requiredRule("char")],
};
const fieldConfig = {
  folder: {
    label: "folder location",
    placeholder: "./example",
    clearable: true,
    ...defaultFieldConfig,
  },
  regex: {
    label: "regex",
    placeholder: "a*b",
    ...defaultFieldConfig,
  },
  searchedChar: {
    label: "char",
    placeholder: "a",
    ...defaultFieldConfig,
  },
};
const buttonConfig = {
  color: "primary",
  label: "Submit",
  size: "lg",
  type: "submit",
};

const onSubmit = () => {
  isLoading.value = true;
  window.ipcRenderer.send("execMain", {
    folderLocation: folderLocation.value,
    regex: regex.value || "noRegex",
    searchedChar: searchedChar.value || "noChar",
  });
};

const matches = ref([]);
const discovered = ref([]);

window.ipcRenderer.receive("result", (data) => {
  const [matchedFiles, discoveredFiles] = data
    .split("&&")
    .map((files) => files.split("\n"));

  matches.value = matchedFiles.map((file) => {
    const [path, line, index] = file.split(":");
    return { path, line, index };
  });
  discovered.value = discoveredFiles.map((file) => {
    const [time, path] = file.split(":");
    return { time, path };
  });

  matches.value = matches.value.filter((file) => file.path);
  discovered.value = discovered.value.filter((file) => file.time);

  isLoading.value = false;
});
</script>

<style scoped lang="scss">
.grep {
  display: grid;
  grid-template-columns: 1fr 2fr;
  width: 100vw;
  height: 100vh;
}
.folder-selection {
  display: flex;
  align-items: center;
  justify-content: center;
  flex-direction: column;
  gap: 3.6rem;
  &__title {
    color: $primary;
  }
  &__form {
    display: flex;
    justify-content: center;
    flex-direction: column;
    gap: 2rem;
    width: 32rem;
  }
  &__toggle {
    display: flex;
    align-items: center;
    color: $input-label-color;
    font-size: 20px;
  }
}
.output {
  width: 100%;
  height: 100%;
  display: flex;
  align-items: flex-start;
  justify-content: flex-start;
  flex-direction: column;
  font-size: 20px;
  color: $primary;
  background-color: rgba($color: $primary, $alpha: 0.2);
  padding: 80px;

  &__charts {
    display: flex;
    gap: 40px;
  }
}
</style>
