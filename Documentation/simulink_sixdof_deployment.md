# Building and Deploying the sixdof_simulator (Simulink → ROS 2) to the RPi

This guide covers the complete workflow to generate a standalone ROS 2 node from the Simulink 6-DOF rocket simulator, build it, and deploy it to the Raspberry Pi companion computer.

---

## 1. Overview

Two Simulink models exist, each with a different purpose:

| Model | Location | Purpose |
|---|---|---|
| `HIL_simulation_Run/sixdof_simulator.slx` | Interactive (Monitor & Tune) | Run the model from MATLAB with scopes, no code generation |
| `HIL_simulation_Compile/sixdof_simulator.slx` | Code generation | Produces the standalone `sixdof_simulator` ROS 2 node (`.tgz` archive) |

Code generation always happens on the **dev machine** (x86_64 Linux, MATLAB + ROS Toolbox + Embedded Coder). The resulting archive is either:

- Built **locally** into `workspace_simulink/` (PC, x86_64), or
- Copied to the **RPi** and built natively there (ARM), because Simulink-generated code must be compiled for the target architecture.

```
MATLAB / Simulink (dev machine, x86_64)
    │  Ctrl+B  (codegen, ExtMode OFF)
    ▼
sixdof_simulator.tgz
    ├───► extract → workspace_simulink/src  ──► colcon build (PC, x86_64)
    │
    └───► scp to RPi ──► extract → workspace_erocket/src ──► colcon build (RPi, ARM)
```

---

## 2. Prerequisites

### Dev machine (PC)
- MATLAB R2026a with **Simulink**, **Simulink Coder / Embedded Coder**, **ROS Toolbox**, and the **ROS 2** support package.
- Ubuntu 24.04 with **ROS 2 Jazzy** (`/opt/ros/jazzy`).
- The hand-written workspace built at least once:
  ```zsh
  cd workspace_erocket
  source /opt/ros/jazzy/setup.zsh
  colcon build
  source install/local_setup.zsh
  ```

> [!NOTE]
> I use zsh, just replace .zsh for .bash if using the bash shell

### Raspberry Pi
- Ubuntu 24.04 with ROS 2 Jazzy and the E-Rocket repo at
  `~/E/E-Rocket-flight-software/` (physical path `/home/ubuntu/E_Rocket_SW/E-Rocket-flight-software`).
- Build tools installed (required for `vrpn_vendor` and C++ packages):
  ```bash
  sudo apt install build-essential cmake
  ```
- SSH access from the dev machine: `ubuntu@10.42.0.13` (hostname `erocket-1`).

---

## 3. Configure the Model for Standalone Codegen (External Mode OFF)

> **Do not skip this.** Generating with External Mode ON produces `rtiostream*`, `ext_svr.c`, `ext_work.c`, and `updown.c` sources that fail to build (missing `rtiostream.h`, undefined `ExtGetHostPkt`, …).

Run the configuration script **once per model / MATLAB session** — it sets the active configuration set, disables external mode, and saves the model:

```matlab
% From the project folder:
cd('/home/sofia/Documents/E-Rocket-All-Stuff/HIL_simulation_Compile')
open_system('sixdof_simulator')
sixdof_sim_codegen        % <-- HIL_simulation_Compile/sixdof_sim_codegen.m
```

The script (`HIL_simulation_Compile/sixdof_sim_codegen.m`) does:

```matlab
cs = getActiveConfigSet('sixdof_simulator');
set_param(cs, 'ExtMode', 'off');
set_param(cs, 'MatFileLogging', 'off');
set_param(cs, 'HardwareBoard', 'Robot Operating System 2 (ROS 2)');  % detaches rtiostream hooks
save_system('sixdof_simulator');
```

**Check it worked** — in Configuration Parameters, confirm:
- **Solver** → Type: **Fixed-step** (required for codegen).
- **Code Generation** → **ROS 2** → the board is **Robot Operating System 2 (ROS 2)**.
- **External mode** / **Monitor & Tune** is disabled.

---

## 4. Target Architecture (PC vs RPi)

The generated code must match the CPU where the node will run.

| Target | Hardware Implementation settings |
|---|---|
| **PC (x86_64)** | Device vendor: **AMD** → Device type: `x86-64 (Linux 64)` *(default)* |
| **RPi (ARM)** | Device vendor: **ARM Compatible** → Device type: **ARM Cortex** |

Change it at **Configuration Parameters → Hardware Implementation** *before* generating. If you generate with x86 settings and deploy to the RPi, the build fails with `emmintrin.h: No such file or directory` (x86 SSE2 intrinsics don't exist on ARM).

---

## 5. Generate the Code (Ctrl+B)

With `HIL_simulation_Compile/sixdof_simulator.slx` open:

1. **Configuration Parameters → Code Generation → ROS 2**:
   - **Package folder**: `sixdof_simulator`
   - **Colcon workspace**: your chosen workspace (see §6/§7).
2. Press **Ctrl+B** (Build). Simulink generates the C++ ROS 2 package and packages it as `sixdof_simulator.tgz` in the MATLAB working folder (`HIL_simulation_Compile/`).

**Verify the archive before deploying** — it must contain `multiword_types.h` and **must not** contain external-mode files:

```bash
cd /home/sofia/Documents/E-Rocket-All-Stuff/HIL_simulation_Compile
tar tzf sixdof_simulator.tgz | grep -E "multiword_types|rtiostream|ext_svr|ext_work|updown"
# EXPECTED:  multiword_types.h  present
#            NO rtiostream / ext_svr / ext_work / updown entries
```

If external-mode files are present, the `sixdof_sim_codegen.m` configuration did not take effect — re-run §3 and regenerate.

> **Never generate directly into `workspace_erocket/`.** Codegen overwrites `CMakeLists.txt`, `package.xml`, and `packageInfo.mat` of the `px4_msgs` and `erocket` packages with stub versions (version `0.0.0`, hardcoded message lists) and deletes source files. Always generate into a dedicated folder/workspace, then deploy from the archive.

---

## 6. Build on the PC (x86_64)

The `workspace_simulink/` workspace holds the PC build of `sixdof_simulator`. It needs `workspace_erocket` as an **underlay** because the generated package depends on the `erocket` and `px4_msgs` message packages.

```zsh
cd /home/sofia/Documents/E-Rocket-All-Stuff/E-Rocket-flight-software

# 1. (Re)create the workspace src/ if needed
mkdir -p workspace_simulink/src

# 2. Extract the archive
tar -xzf HIL_simulation_Compile/sixdof_simulator.tgz -C workspace_simulink/src/

# 3. Build with workspace_erocket as underlay
source /opt/ros/jazzy/setup.zsh
source workspace_erocket/install/local_setup.zsh
cd workspace_simulink
colcon build
source install/local_setup.zsh
```

**Run on the PC:**

```zsh
# From workspace_erocket (launch file lives in the erocket package)
source install/local_setup.zsh
source ../workspace_simulink/install/local_setup.zsh
ros2 launch erocket sitl_sixdof.launch.py
```

### 6.1 Rebuilding only `sixdof_simulator` (skip `px4_msgs` / `erocket`)

`px4_msgs` takes 3–4 minutes to compile. Once it and `erocket` are installed, you can rebuild **only** the simulator node in seconds using `--packages-select` — colcon reuses the already-installed packages from `install/` and never recompiles them:

```zsh
cd /home/sofia/Documents/E-Rocket-All-Stuff/E-Rocket-flight-software/workspace_simulink
source /opt/ros/jazzy/setup.zsh
source ../workspace_erocket/install/local_setup.zsh   # underlay (messages)
colcon build --packages-select sixdof_simulator
source install/local_setup.zsh
```

On the RPi (single workspace, no underlay needed):

```bash
cd ~/E/E-Rocket-flight-software/workspace_erocket
source /opt/ros/jazzy/setup.zsh
colcon build --packages-select sixdof_simulator
source install/local_setup.zsh
```

**Speed-up:** `workspace_simulink/src/` may contain stale copies of `erocket`/`px4_msgs` (provided by the underlay anyway). Delete them so colcon doesn't even discover them — builds stay fast and Simulink codegen can never corrupt them again:

```zsh
rm -rf workspace_simulink/src/erocket workspace_simulink/src/px4_msgs
```

**Related flags:**
- `colcon build --packages-up-to sixdof_simulator` — builds it *plus* everything it depends on (use after a fresh `rm -rf install/`).
- `colcon build --packages-above sixdof_simulator` — builds everything that *depends on* it.
- Avoid `rm -rf install/` for routine rebuilds — it forces the full workspace rebuild including the slow `px4_msgs`.

---

## 7. Deploy and Build on the RPi (ARM)

> The RPi builds the archive natively — do **not** copy the PC build's binaries. Only the `.tgz` crosses the network.

### 7.1 Copy the archive

```zsh
scp /home/sofia/Documents/E-Rocket-All-Stuff/HIL_simulation_Compile/sixdof_simulator.tgz \
    ubuntu@10.42.0.13:~/E/E-Rocket-flight-software/
```

### 7.2 Extract and build on the RPi

```bash
cd ~/E/E-Rocket-flight-software

# Remove any previous (possibly stale/corrupted) copy
rm -rf workspace_erocket/src/sixdof_simulator

# Extract the ARM archive into the hand-written workspace
tar -xzf sixdof_simulator.tgz -C workspace_erocket/src/

cd workspace_erocket
source /opt/ros/jazzy/setup.zsh
colcon build          # builds px4_msgs, erocket, ..., sixdof_simulator
source install/local_setup.zsh
```

### 7.3 Run on the RPi

```bash
ros2 launch erocket sitl_sixdof.launch.py
```

Trigger the mission phases via `ros2 param set`:

```bash
ros2 param set /mission offboard.flight_mode 3   # TAKE_OFF
ros2 param set /mission offboard.flight_mode 4   # IN_MISSION
ros2 param set /mission offboard.flight_mode 5   # LAND
```

---

## 8. Common Pitfalls and Troubleshooting

### P1. Build fails: `multiword_types.h: No such file or directory`
**Symptom:**
```
fatal error: multiword_types.h: No such file or directory
```
**Cause:** the Simulink archive packaging omitted a generated header.

**Fix:** copy the header from an existing build (it is architecture-agnostic typedefs):
```bash
scp /home/sofia/Documents/E-Rocket-All-Stuff/E-Rocket-flight-software/workspace_simulink/src/sixdof_simulator/include/sixdof_simulator/multiword_types.h \
    ubuntu@10.42.0.13:~/E/E-Rocket-flight-software/workspace_erocket/src/sixdof_simulator/include/sixdof_simulator/
```
**Prevention:** check the archive with the `tar tzf` command in §5 before deploying; regenerate if the header is missing.

### P2. Build fails on external-mode files
**Symptom (compile):**
```
rtiostream_tcpip.c:32: fatal error: rtiostream.h: No such file or directory
```
**Symptom (link):**
```
ext_svr.c: undefined reference to `ExtGetHostPkt'
ext_svr.c: undefined reference to `UploadBufGetData'
```
**Cause:** code was generated with External Mode (Monitor & Tune) enabled; the archive references `rtiostream` headers that are never packaged.

**Fix (proper):** run `sixdof_sim_codegen.m` (§3) and regenerate — the archive will not contain these files.

**Fix (surgical, if you must keep the archive):** strip external-mode sources and the define from `CMakeLists.txt`:
```bash
sed -i '/rtiostream/d; /ext_svr/d; /ext_work/d; /updown/d; /EXT_MODE/d; /include.*rtiostream/d' \
    workspace_erocket/src/sixdof_simulator/CMakeLists.txt
```
Then rebuild (`colcon build`). Note: `-fpermissive` warnings for C files in this CMakeLists are harmless.

### P3. Build fails on RPi: `emmintrin.h: No such file or directory`
**Symptom:** x86 SSE2 intrinsic headers missing on ARM.
**Cause:** code was generated with x86 (PC) hardware settings, then built on ARM.
**Fix:** regenerate with **Hardware Implementation → ARM Compatible / ARM Cortex** (§4), re-archive, re-deploy.

### P4. Simulink codegen corrupted `px4_msgs` / `erocket` packages
**Symptom:** after codegen, `colcon build` fails, or `px4_msgs` reports `Rpm.msg: No such file or directory`, or packages show version `0.0.0` / `TODO` metadata.
**Cause:** Simulink codegen overwrote `CMakeLists.txt`, `package.xml`, and `packageInfo.mat` with stub versions whenever it was pointed at (or extracted into) a workspace containing these packages.

**Fix:** restore the real packages from the pristine copy and clean stale build state:
```zsh
cd /home/sofia/Documents/E-Rocket-All-Stuff/E-Rocket-flight-software
rm -rf workspace_simulink/src/px4_msgs workspace_simulink/src/erocket
cp -r workspace_erocket/src/px4_msgs workspace_simulink/src/
cp -r workspace_erocket/src/erocket workspace_simulink/src/
rm -rf workspace_simulink/build workspace_simulink/install workspace_simulink/log
cd workspace_simulink
source /opt/ros/jazzy/setup.zsh
source ../workspace_erocket/install/local_setup.zsh
colcon build
```
**Prevention:** never let codegen write into a workspace that already contains `px4_msgs`/`erocket`. On the PC, prefer a dedicated `workspace_simulink/` with only the generated package; on the RPi, restore the packages after a bad deploy with:
```bash
git checkout HEAD -- src/erocket src/px4_msgs    # inside the repo
```

### P5. `sixdof_simulator` package is missing after codegen
**Symptom:** `colcon build` no longer sees the package (`ls src/` shows only `erocket`/`px4_msgs`).
**Cause:** codegen deleted the previously generated package in the target workspace.

**Fix:** re-extract the archive (§6.2 / §7.2). If you restore from git instead, note that the committed version still contains external-mode files — apply P2's surgical fix or regenerate.

### P6. Colcon skips packages silently (`COLCON_IGNORE` files)
**Symptom:** a package is never built, with no error.
**Cause:** Simulink scatters `COLCON_IGNORE` files into the workspace during codegen.

**Fix:**
```zsh
find .. -name "COLCON_IGNORE" -delete
```

### P7. `px4_msgs` builds take minutes or fail with stale artifacts
**Symptom:** build hangs on `[Processing: px4_msgs]` for many minutes, or install fails copying `.msg` files.
**Cause:** stale/corrupted `build/`, `install/`, `log/` from previous codegen runs.

**Fix:** `rm -rf build install log` and rebuild (see P4 for the full sequence).

### P8. Simulink scopes show no data (interactive model only)
**Symptom:** the compiled node works, but `HIL_simulation_Run` scopes are flat.
**Cause:** **bus definition mismatch** in `HIL_simulation_Run/+bus_conv_fcns` — the Subscribe block silently drops messages when the bus layout differs from the `.msg` (e.g., fields like `thrust`, `inner_servo_tilt_angle_deg`, `outer_servo_tilt_angle_deg`, `delta_motor_pwm` added to `GenericControllerDebug.msg` without syncing the bus).

**Fix:** keep the bus and `.msg` in sync:
1. If the `.msg` is the source of truth: delete `+bus_conv_fcns/`, reopen the model, and let Simulink regenerate the bus conversion functions.
2. If Simulink needs fields the `.msg` lacks: add them to `workspace_erocket/src/erocket/msg/GenericControllerDebug.msg` **in the same order as the bus**, rebuild the workspace, and populate the fields in the C++ publisher.

### P9. Interactive model cannot connect to ROS 2
**Symptom:** "ROS 2 node not initialized" or topics missing.
**Fix:** source the workspace, start MATLAB from the same terminal, and create a ROS 2 node before running the model:
```matlab
node = ros2node("/simulink_sitl");
```

### P10. RPi build fails on `vrpn_vendor` or missing C++ toolchain
**Symptom:** `colcon build` fails early with compiler/CMake errors unrelated to `sixdof_simulator`.
**Fix:**
```bash
sudo apt install build-essential cmake
```

### P11. Correct workspace sourcing order
The generated package depends on `erocket` and `px4_msgs` messages. Always source the underlay first, the generated workspace second:

```zsh
source /opt/ros/jazzy/setup.zsh
source workspace_erocket/install/local_setup.zsh     # underlay (messages)
source workspace_simulink/install/local_setup.zsh    # generated node (PC only)
```
On the RPi everything lives in one workspace, so only the ROS 2 setup and `install/local_setup.zsh` are needed.

---

## Useful Checks

```bash
# Is the node present after building?
ros2 pkg executables sixdof_simulator

# Is it running and what does it publish/subscribe?
ros2 node list
ros2 node info /sixdof_simulator

# Are the simulation topics flowing?
ros2 topic list | grep /fmu/out
```
