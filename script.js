/* =========================================================
   STUDENT HUB - STUDENT RECORD MANAGEMENT SYSTEM
   Complete corrected script.js
   ========================================================= */


/* ================= STUDENT DATA ================= */

let students = [

    {
        roll: 101,
        name: "Riya Sharma",
        email: "riya@gmail.com",
        phone: "9876543210",
        department: "EXTC",
        year: "TE",
        division: "A",
        cgpa: 8.7,
        attendance: 92,
        status: "Active"
    },

    {
        roll: 102,
        name: "Ananya Patil",
        email: "ananya@gmail.com",
        phone: "9876543211",
        department: "CSE",
        year: "TE",
        division: "B",
        cgpa: 9.2,
        attendance: 95,
        status: "Active"
    },

    {
        roll: 103,
        name: "Rahul Shah",
        email: "rahul@gmail.com",
        phone: "9876543212",
        department: "IT",
        year: "SE",
        division: "A",
        cgpa: 8.1,
        attendance: 86,
        status: "Active"
    },

    {
        roll: 104,
        name: "Sneha More",
        email: "sneha@gmail.com",
        phone: "9876543213",
        department: "EXTC",
        year: "BE",
        division: "A",
        cgpa: 7.6,
        attendance: 72,
        status: "Active"
    },

    {
        roll: 105,
        name: "Aarav Mehta",
        email: "aarav@gmail.com",
        phone: "9876543214",
        department: "AI & DS",
        year: "TE",
        division: "A",
        cgpa: 9.5,
        attendance: 97,
        status: "Active"
    }

];


let deletedStudents = [];

let waitingStudents = [
    "Neha Kulkarni",
    "Arjun Desai"
];

let editingIndex = -1;


/* =========================================================
   HELPER FUNCTIONS
   ========================================================= */

/* Safely convert a value into a number */

function safeNumber(value, defaultValue = 0) {

    const number = Number(value);

    return Number.isFinite(number)
        ? number
        : defaultValue;
}


/* Safely get an element */

function getElement(id) {

    return document.getElementById(id);

}


/* Safely get value from an input */

function getInputValue(id) {

    const element = getElement(id);

    if (!element) {
        return "";
    }

    return element.value.trim();

}


/* =========================================================
   SECTION NAVIGATION
   ========================================================= */

function showSection(sectionId, button) {

    const sections =
        document.querySelectorAll(".page-section");

    sections.forEach(section => {

        section.classList.remove("active-section");

    });


    const selected =
        getElement(sectionId);

    if (selected) {

        selected.classList.add("active-section");

    }


    const buttons =
        document.querySelectorAll(".nav-item");

    buttons.forEach(btn => {

        btn.classList.remove("active");

    });


    if (button) {

        button.classList.add("active");

    }
    else {

        buttons.forEach(btn => {

            const onclick =
                btn.getAttribute("onclick");

            if (
                onclick &&
                onclick.includes(`'${sectionId}'`)
            ) {

                btn.classList.add("active");

            }

        });

    }


    const names = {

        dashboard: "Dashboard",
        students: "Students",
        academics: "Academics",
        attendance: "Attendance",
        courses: "Courses",
        waiting: "Waiting List",
        deleted: "Recently Deleted",
        reports: "Reports & Analytics",
        dsa: "DSA Operations",
        administration: "Administration",
        notifications: "Notifications",
        settings: "Settings"

    };


    const pageName =
        getElement("pageName");

    const pageTitle =
        getElement("pageTitle");


    if (pageName) {

        pageName.textContent =
            names[sectionId] || "Dashboard";

    }


    if (pageTitle) {

        pageTitle.textContent =
            names[sectionId] || "Dashboard";

    }


    window.scrollTo({

        top: 0,
        behavior: "smooth"

    });


    if (sectionId === "academics") {

        updateAcademicSection();

    }


    if (sectionId === "attendance") {

        updateAttendance();

    }


    if (sectionId === "reports") {

        updateReports();

    }


    if (sectionId === "waiting") {

        updateWaitingList();

    }


    if (sectionId === "deleted") {

        updateDeletedList();

    }

}


/* =========================================================
   STUDENT MODAL
   ========================================================= */

function openStudentModal() {

    const modal =
        getElement("studentModal");

    if (modal) {

        modal.classList.add("show");

    }

}


function closeStudentModal() {

    const modal =
        getElement("studentModal");

    if (modal) {

        modal.classList.remove("show");

    }


    const form =
        getElement("studentForm");

    if (form) {

        form.reset();

    }


    editingIndex = -1;


    const modalTitle =
        getElement("modalTitle");

    if (modalTitle) {

        modalTitle.textContent =
            "Add New Student";

    }

}


/* =========================================================
   SAVE STUDENT
   ========================================================= */

function saveStudent(event) {

    event.preventDefault();


    /*
       IMPORTANT:
       We read the values directly from the student form.
       This prevents the wrong attendance field from being
       selected if another element has a similar ID.
    */

    const form =
        getElement("studentForm");


    if (!form) {

        alert("Student form not found.");

        return;

    }


    /* ---------- ROLL NUMBER ---------- */

    const rollElement =
        form.querySelector("#roll");

    const rollText =
        rollElement
            ? rollElement.value.trim()
            : "";

    const roll =
        Number(rollText);


    if (
        rollText === "" ||
        !Number.isFinite(roll) ||
        roll <= 0
    ) {

        alert(
            "Please enter a valid Roll Number."
        );

        return;

    }


    /* ---------- NAME ---------- */

    const nameElement =
        form.querySelector("#name");

    const name =
        nameElement
            ? nameElement.value.trim()
            : "";


    if (name === "") {

        alert(
            "Please enter the student's name."
        );

        return;

    }


    /* ---------- EMAIL ---------- */

    const emailElement =
        form.querySelector("#email");

    const email =
        emailElement
            ? emailElement.value.trim()
            : "";


    if (email === "") {

        alert(
            "Please enter the student's email."
        );

        return;

    }


    /* ---------- PHONE ---------- */

    const phoneElement =
        form.querySelector("#phone");

    const phone =
        phoneElement
            ? phoneElement.value.trim()
            : "";


    if (phone === "") {

        alert(
            "Please enter the student's phone number."
        );

        return;

    }


    /* ---------- DEPARTMENT ---------- */

    const departmentElement =
        form.querySelector("#department");

    const department =
        departmentElement
            ? departmentElement.value
            : "";


    /* ---------- YEAR ---------- */

    const yearElement =
        form.querySelector("#year");

    const year =
        yearElement
            ? yearElement.value
            : "";


    /* ---------- DIVISION ---------- */

    const divisionElement =
        form.querySelector("#division");

    const division =
        divisionElement
            ? divisionElement.value
            : "";


    /* =====================================================
       CGPA
       ===================================================== */

    const cgpaElement =
        form.querySelector("#cgpa");


    const cgpaText =
        cgpaElement
            ? cgpaElement.value.trim()
            : "";


    const cgpa =
        parseFloat(cgpaText);


    if (
        cgpaText === "" ||
        !Number.isFinite(cgpa) ||
        cgpa < 0 ||
        cgpa > 10
    ) {

        alert(
            "Please enter a valid CGPA between 0 and 10."
        );

        return;

    }


    /* =====================================================
       ATTENDANCE
       ===================================================== */

    const attendanceElement =
        form.querySelector("#attendance");


    const attendanceText =
        attendanceElement
            ? attendanceElement.value.trim()
            : "";


    const attendance =
        parseFloat(attendanceText);


    /*
       THIS FIXES THE NaN PROBLEM.

       Empty values, text values and invalid numbers
       are rejected before saving.
    */

    if (
        attendanceText === "" ||
        !Number.isFinite(attendance) ||
        attendance < 0 ||
        attendance > 100
    ) {

        alert(
            "Please enter a valid attendance between 0 and 100."
        );

        return;

    }


    /* ---------- STATUS ---------- */

    const statusElement =
        form.querySelector("#status");

    const status =
        statusElement
            ? statusElement.value
            : "Active";


    /* =====================================================
       CREATE STUDENT OBJECT
       ===================================================== */

    const student = {

        roll: roll,

        name: name,

        email: email,

        phone: phone,

        department: department,

        year: year,

        division: division,

        cgpa: cgpa,

        attendance: attendance,

        status: status

    };


    /* =====================================================
       ADD OR UPDATE
       ===================================================== */

    if (editingIndex === -1) {

        students.push(student);

        alert(
            "Student added successfully!"
        );

    }
    else {

        students[editingIndex] =
            student;

        alert(
            "Student record updated successfully!"
        );

    }


    /* ---------- CLOSE MODAL ---------- */

    closeStudentModal();


    /* ---------- REFRESH WEBSITE ---------- */

    refreshAll();

}


/* =========================================================
   DISPLAY STUDENTS
   ========================================================= */

function displayStudents(list = students) {

    const table =
        getElement("studentTable");


    if (!table) {

        return;

    }


    table.innerHTML = "";


    if (list.length === 0) {

        table.innerHTML = `

            <tr>

                <td colspan="9"
                    style="text-align:center;padding:30px">

                    No student records found.

                </td>

            </tr>

        `;

        return;

    }


    list.forEach(student => {

        const index =
            students.indexOf(student);


        const row =
            document.createElement("tr");


        const statusClass =
            student.status === "Active"
                ? "status-active"
                : "status-inactive";


        /* Safe CGPA */

        const cgpa =
            safeNumber(student.cgpa);


        /* Safe Attendance */

        const attendance =
            safeNumber(student.attendance);


        row.innerHTML = `

            <td>
                ${student.roll}
            </td>

            <td>

                <span class="student-name">
                    ${student.name}
                </span>

            </td>

            <td>
                ${student.department}
            </td>

            <td>
                ${student.year}
            </td>

            <td>
                ${student.division}
            </td>

            <td>

                <span class="cgpa-value">
                    ${cgpa.toFixed(2)}
                </span>

            </td>

            <td>
                ${attendance.toFixed(0)}%
            </td>

            <td>

                <span class="${statusClass}">
                    ${student.status}
                </span>

            </td>

            <td>

                <button
                    class="table-action"
                    onclick="editStudent(${index})">

                    ✏️

                </button>


                <button
                    class="table-action"
                    onclick="deleteStudent(${index})">

                    🗑️

                </button>

            </td>

        `;


        table.appendChild(row);

    });

}


/* =========================================================
   EDIT STUDENT
   ========================================================= */

function editStudent(index) {

    const student =
        students[index];


    if (!student) {

        return;

    }


    editingIndex = index;


    const roll =
        getElement("roll");

    const name =
        getElement("name");

    const email =
        getElement("email");

    const phone =
        getElement("phone");

    const department =
        getElement("department");

    const year =
        getElement("year");

    const division =
        getElement("division");

    const cgpa =
        getElement("cgpa");

    const attendance =
        getElement("attendance");

    const status =
        getElement("status");


    if (roll) {

        roll.value =
            student.roll;

    }


    if (name) {

        name.value =
            student.name;

    }


    if (email) {

        email.value =
            student.email;

    }


    if (phone) {

        phone.value =
            student.phone;

    }


    if (department) {

        department.value =
            student.department;

    }


    if (year) {

        year.value =
            student.year;

    }


    if (division) {

        division.value =
            student.division;

    }


    if (cgpa) {

        cgpa.value =
            safeNumber(student.cgpa);

    }


    if (attendance) {

        attendance.value =
            safeNumber(student.attendance);

    }


    if (status) {

        status.value =
            student.status;

    }


    const modalTitle =
        getElement